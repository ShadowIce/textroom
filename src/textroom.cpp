/****************************************************************************
** Copyright (C) 2008 Petar Toushkov <peter dot toushkov at gmail.com>
** Copyright (C) 2008 Omer Bahri Gordebak <gordebak at gmail.com>
**
** Additional help, code and insights by:
** adamvert - from http://ubuntuforums.org/
** zebulon M - from http://ubuntuforums.org/
**
** Artwork by Edward Solorukhin <arch1000@gmail.com>
**
** Parts of the following code are from the Phrasis project:
**
** http://code.google.com/p/phrasis/
**
** and are
**
** Copyright (C) 2006 Jacob R Rideout <jacob@jacobrideout.net>
** All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui> 
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "textroom.h"
#include "optionsdialog.h"
#include "helpdialog.h"
#include "searchdialog.h"
#include "font.h"

	
TextRoom::TextRoom(QWidget *parent, Qt::WFlags f)
		: QWidget(parent, f), sentenceTally(0),
		sdlInitialized(false), sdlFailed(false)
{
	setupUi(this);
	setObjectName("textroom");

	readSettings();

	numChanges = 0;
	prevLength = 0;
	wordcount = 0;
	alarm = 0;
	parasold = 0;

	optionsDialog = new OptionsDialog(this);
	helpDialog = new HelpDialog(this);
	selectFont = new SelectFont(this);
	
	new QShortcut ( QKeySequence(QKeySequence::New), this, SLOT( newFile() ) );
	new QShortcut ( QKeySequence(QKeySequence::Open), this, SLOT( open() ) );
	new QShortcut ( QKeySequence(QKeySequence::Save), this, SLOT( save() ) );
	new QShortcut ( QKeySequence(QKeySequence::HelpContents), this, SLOT( help() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+P", "Options")), this, SLOT( options() ) );
	new QShortcut ( QKeySequence(tr("Shift+Ctrl+S", "Save As")), this, SLOT( saveAs() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+D", "Insert Date")), this, SLOT( insertDate() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+T", "Insert Time")), this, SLOT( insertTime() ) );	
	new QShortcut ( QKeySequence(tr("Ctrl+H", "About TextRoom")), this, SLOT( about() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+Q", "Quit Application")) , this, SLOT( close() ) );
	new QShortcut ( QKeySequence(tr("Alt+F4", "Quit Application")) , this, SLOT( close() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+F", "Find Text")) , this, SLOT( find() ) );
	new QShortcut ( QKeySequence(tr("F11", "Toggle Fullscreen")) , this, SLOT( toggleFullScreen() ) );
	new QShortcut ( QKeySequence(tr("Esc", "Toggle Fullscreen")) , this, SLOT( toggleEscape() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+M", "Minimize TextRoom")) , this, SLOT( showMinimized() ) );
	new QShortcut ( QKeySequence(tr("F3", "Find Next")) , this, SLOT( find_next() ) );
	new QShortcut ( QKeySequence(tr("Shift+F3", "Find Previous")) , this, SLOT( find_previous() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+B", "Bold")) , this, SLOT( textBold() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+I", "Italic")) , this, SLOT( textItalic() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+U", "Find Previous")) , this, SLOT( textUnderline() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+Up", "Increase Text Size")) , this, SLOT( textSizeUp() ) );
	new QShortcut ( QKeySequence(tr("Ctrl+Down", "Decrease Text Size")) , this, SLOT( textSizeDown() ) );	
	new QShortcut ( QKeySequence(tr("Ctrl+W", "Select Font")) , this, SLOT( changeFont() ) );	
	
	// Service: show cursor
	new QShortcut ( QKeySequence(tr("Shift+F4", "Show Cursor")) , this, SLOT( sCursor() ) );

#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else
	QSettings settings;
#endif	

	fw = new QFileSystemWatcher(this);
	fw->addPath( settings.fileName() );

	connect(fw, SIGNAL(fileChanged(const QString)),
			this, SLOT(readSettings()));
	
	connect(textEdit->document(), SIGNAL(contentsChanged()),
			this, SLOT(documentWasModified()));

	connect(textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)),
			this, SLOT(vPositionChanged()));

	connect(horizontalSlider, SIGNAL(valueChanged(int)),
			this, SLOT(hSliderPositionChanged()));

	// check if we need to open some file at startup
	const QStringList args = QCoreApplication::arguments();
	if (args.count() == 2)
	{
		QFile file(args.at(1));
			if (file.exists()) curFile = args.at(1);
	}
	
	if (!curFile.isEmpty()) loadFile(curFile);
	else newFile();

	// set cursor position
	if ( isSaveCursor )
	{
		QTextCursor c;
		c = textEdit->textCursor();
		c.setPosition(cPosition);
		textEdit->setTextCursor(c);
	}

	writeSettings();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(getFileStatus()));
	timer->start(1000);
}

TextRoom::~TextRoom() {
	if (sdlInitialized) destroySDLMixer();
}


void TextRoom::toggleEscape()
{
	if (helpDialog->isVisible()) helpDialog->hide();
	else if (isFullScreen()) toggleFullScreen();
	else close();
}

void TextRoom::insertDate()
{
	QDateTime today = QDateTime::currentDateTime();
	QString date = today.toString(dateFormat);
	setWindowModified(textEdit->document()->isModified());

	const QString text( textEdit->document()->toPlainText() );

	textEdit->insertPlainText(date);
}

void TextRoom::insertTime()
{
	QDateTime now = QDateTime::currentDateTime();
	QString clock = now.toString(timeFormat);
	setWindowModified(textEdit->document()->isModified());

	const QString text( textEdit->document()->toPlainText() );

	textEdit->insertPlainText(clock);
}

void TextRoom::toggleFullScreen()
{
	if (!isFullScreen()) showFullScreen();
	else showNormal();
//	sCursor();
}

void TextRoom::closeEvent(QCloseEvent *event)
{
//	qDebug() << "closing";
	if (maybeSave())
	{
		fw->disconnect();

		// save cursor position
		if ( isSaveCursor )
		{
			QTextCursor c;
			c = textEdit->textCursor();
			cPosition = c.position();
		}
		wordcount = 0;
		alarm = 0;
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}
 
void TextRoom::about() 
{
	QMessageBox::about(this,"About TextRoom",
				"TextRoom Editor ver. 0.2.5\n\n"
		"Project home page: http://code.google.com/p/textroom/\n\n"
		"Code, help and insights (in alphabetical order) by:\n"
		"Adamvert (from ubuntuforums.org),\n"
		"Magnus Edvardsson (a.k.a. zebulon M),\n"
		"Omer Bahri Gordebak,\n"
		"Peter Toushkov\n\n"
		"TextRoom is partially based on\n"
		"The Phrasis project by Jacob R. Rideout:\n"
		"http://code.google.com/p/phrasis/\n\n");
}

void TextRoom::newFile()
{
	if (maybeSave())
	{

		textEdit->document()->blockSignals(true);
		textEdit->clear();
		textEdit->document()->blockSignals(false);
		
		setCurrentFile("");
		textEdit->setUndoRedoEnabled(true);
		textEdit->document()->setModified(false);
		textEdit->verticalScrollBar()->setValue(0);
		textEdit->setFont(defaultFont);
	}
}

void TextRoom::open()
{
	if (maybeSave())
	{
		QString dirToOpen;
		if ( curDir.isEmpty() )
		{
			dirToOpen = QDir::homePath();
	   	}
	   	else
	   	{
	   		dirToOpen = curDir;
	  	}
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), dirToOpen);
		if (!fileName.isEmpty())
		{
			loadFile(fileName);
	   	}
	}
}

bool TextRoom::save()
{
	if (curFile.isEmpty() || !QFileInfo(curFile).isWritable() || label->text().startsWith("Untitled"))
	{
		return saveAs();
	}
	else
	{
		return saveFile(curFile);
	}
}

bool TextRoom::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save As", QDir::homePath() + "/*.txr", "TextRoom Documents (*.txr);;Html Files (*.htm *.html);;Text Documents (*.txt)");
	if (!fileName.isEmpty())
	{
		return saveFile(fileName);
	}
	else
	{
		return true;
	}
}

void TextRoom::autoSave()
{
	if (label->text().startsWith("Untitled"))
	{
		QString fileName = QDir::homePath() + "/UntitledAutoSaved.txr";
		saveFile(fileName);
	}
	else
	{
		save();
	}
}

void TextRoom::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, qApp->applicationName(),
							 tr("Cannot read file %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		newFile();
		return;
	}
	
	QByteArray data = file.readAll();
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QString str = codec->toUnicode(data);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);

	file.seek(0);
	textEdit->document()->blockSignals(true);
	textEdit->setPlainText("");
	textEdit->setUndoRedoEnabled(false);
	textEdit->append(str);
	textEdit->moveCursor(QTextCursor::Start);
	textEdit->setUndoRedoEnabled(true);
	textEdit->document()->blockSignals(false);

	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);

	changeDefaultFont();

	QString text = textEdit->document()->toPlainText();
	parasold = text.count("\n", Qt::CaseSensitive);
	vPositionChanged();
	getFileStatus();

}

bool TextRoom::maybeSave()
{
	if (textEdit->document()->isEmpty())
	{
	return true;
	}
	if (textEdit->document()->isModified())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, qApp->applicationName(),
								   tr("The document has been modified.\n"
									  "Do you want to save your changes?"),
								   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

bool TextRoom::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(this, qApp->applicationName(),
							 tr("Cannot write file %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		return false;
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");

	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (fileName.endsWith("txt"))
	{
		out << textEdit->document()->toPlainText();
	}
	else
	{
		QByteArray ba = "utf-8";
		out << textEdit->document()->toHtml(ba);
	}
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	documentWasModified();	
	return true;
}

void TextRoom::setCurrentFile(const QString &fileName)
{
	curFile = fileName;

	const QFileInfo f(fileName);
	
	textEdit->document()->setModified(false);
	setWindowModified(false);

	QString shownName;
	QString labelToolTip;
	if (curFile.isEmpty())
	{
		shownName = "Untitled.txr";
		labelToolTip = "";
   	}
	else
	{
		shownName = strippedName(curFile);
		labelToolTip = fileName;
		curDir = f.absolutePath();
   	}
	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg( qApp->applicationName() ));
	label->setText(shownName);
	label->setToolTip(labelToolTip);
}

QString TextRoom::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void TextRoom::getFileStatus()
{
	//Deadline
	QString showdeadline;
	QString targetdate = deadline.toString("yyyyMMdd");
	QString targetday = deadline.toString("dd");
	QString targetmonth = deadline.toString("MM");
	QString targetyear = deadline.toString("yyyy");
	QString target;
	int targetdayint = targetday.toInt();
	int targetmonthint = targetmonth.toInt();
	int targetyearint = targetyear.toInt();
	today = QDate::currentDate();
	QString daytoday = today.toString("dd");
	QString thismonth = today.toString("MM");
	QString thisyear = today.toString("yyyy");
	int daytodayint = daytoday.toInt();
	int thismonthint = thismonth.toInt(); 
	int thisyearint = thisyear.toInt();
	int daysremaining = (targetdayint-daytodayint);
	int monthsremaining = targetmonthint-thismonthint;
	int yearsremaining = targetyearint-thisyearint;
	QString daysto;
	daysto.setNum(daysremaining);
	QString monthsto;
	monthsto.setNum(monthsremaining);
	QString yearsto;
	yearsto.setNum(yearsremaining);
	if (yearsremaining < 0)
	{
		showdeadline = "";
		editDate = thisyear+thismonth+daytoday;
	}
	else if (yearsremaining == 0 && monthsremaining <0)
	{
		showdeadline="";
		editDate = thisyear+thismonth+daytoday;
	}
	else if (yearsremaining == 0 && monthsremaining==0)
	{
		if (daysremaining<=0)
		{
			showdeadline="";
			editDate = thisyear+thismonth+daytoday;
		}
		if (daysremaining>0)
		{
			showdeadline = daysto + " Day(s) ";
			editDate = targetdate;
		}
	}
	else if (yearsremaining == 0 && monthsremaining > 0)
	{
		editDate = targetdate;
		if (daysremaining == 0)
		{
			showdeadline = monthsto + " Month(s) ";
		}
		if (daysremaining > 0)
		{
			showdeadline = monthsto + " Month(s), " + daysto + " Day(s) ";
		}
		if (daysremaining < 0)
		{
			QString monthstominusone;
			monthstominusone.setNum(monthsremaining-1);
			QString ismonths;
			if (monthsremaining-1 == 0)
			{
				ismonths = "";
			}
			if (monthsremaining-1 > 0)
			{
				ismonths = monthstominusone + " Month(s), ";
			}
			if (thismonthint == 1 || thismonthint == 3 || thismonthint == 5 || thismonthint == 7 || thismonthint == 8 || thismonthint == 10 ||thismonthint == 12)
			{
				QString daystoplusthirtyone;
				daystoplusthirtyone.setNum(daysremaining+31);
				showdeadline = ismonths + daystoplusthirtyone + " Day(s) ";
			}
			if (thismonthint == 2)
			{
				int daystoplusonefebruary;
				daystoplusonefebruary = daysremaining + 28;
				if (yearsremaining%4==0)
				{
					daystoplusonefebruary = daystoplusonefebruary + 1;
				}
				QString daysforfebruary;
				daysforfebruary.setNum(daystoplusonefebruary);
				showdeadline = ismonths + daysforfebruary + " Day(s) ";
			}
			if (thismonthint == 4 || thismonthint == 6 || thismonthint == 9 || thismonthint == 11)
			{
				int daystoplusthirty;
				daystoplusthirty = daysremaining + 30;
				QString daysforothers;
				daysforothers.setNum(daystoplusthirty);
				showdeadline = ismonths + daysforothers + " Day(s) ";
			}
		}
	}
	else if (yearsremaining > 0 && monthsremaining > 0)
	{
		editDate = targetdate;
		if (daysremaining == 0)
		{
			showdeadline= monthsto + " Month(s)  ";			
		}
		if (daysremaining > 0)
		{
			showdeadline = monthsto + " Month(s), " + daysto + " Day(s) ";			
		}
		if (daysremaining<0)
		{
			QString isyears;
			if (yearsremaining == 0)
			{
				isyears = "";
			}
			else
				isyears = yearsto + " Year(s), ";
			QString monthstominusone;
			monthstominusone.setNum(monthsremaining-1);
			QString ismonths;
			if (monthsremaining-1 == 0)
			{
				ismonths = "";			
			}
			else
			{
				ismonths = monthstominusone + " Month(s), ";
			}
			if (thismonthint == 1 || thismonthint == 3 || thismonthint == 5 || thismonthint == 7 || thismonthint == 8 || thismonthint == 10 ||thismonthint == 12)
			{
				QString daystoplusthirtyone;
				daystoplusthirtyone.setNum(daysremaining+31);
				showdeadline = isyears + ismonths + daystoplusthirtyone + " Day(s) ";
			}
			if (thismonthint == 2)
			{
				int daystoplusonefebruary;
				daystoplusonefebruary = daysremaining + 28;
				if (yearsremaining%4==0)
				{
					daystoplusonefebruary = daystoplusonefebruary + 1;
				}
				QString daysforfebruary;
				daysforfebruary.setNum(daystoplusonefebruary);
				showdeadline = isyears + ismonths + daysforfebruary + " Day(s) ";
			}
			if (thismonthint == 4 || thismonthint == 6 || thismonthint == 9 || thismonthint == 11)
			{
				int daystoplusthirty;
				daystoplusthirty = daysremaining + 30;
				QString daysforothers;
				daysforothers.setNum(daystoplusthirty);
				showdeadline = isyears  + ismonths + daysforothers + " Day(s) ";
			}
		}
	}
	else if (yearsremaining > 0 && monthsremaining == 0)
	{
		editDate = targetdate;
		if (daysremaining == 0)
		{
			showdeadline = yearsto + " Year(s) ";
		}
		if (daysremaining > 0)
		{
			showdeadline = yearsto + " Year(s), " + daysto + " Day(s) "; 
		}
		if (daysremaining < 0)
		{
			QString isyears;
			isyears.setNum(yearsremaining-1);
			QString monthstopluseleven;
			monthstopluseleven.setNum(monthsremaining+11);
			if (yearsremaining-1 == 0)
			{
				isyears="";
			}
			QString ismonths = monthstopluseleven + " Month(s), ";
			if (thismonthint == 1 || thismonthint == 3 || thismonthint == 5 || thismonthint == 7 || thismonthint == 8 || thismonthint == 10 ||thismonthint == 12)
			{
				
				QString daystoplusthirtyone;
				daystoplusthirtyone.setNum(daysremaining+31);
				showdeadline = isyears + ismonths + daystoplusthirtyone + " Day(s) ";
			}
			if (thismonthint == 2)
			{
				int daystoplusonefebruary;
				daystoplusonefebruary = daysremaining + 28;
				if (thisyearint%4==0)
				{
					daystoplusonefebruary = daystoplusonefebruary + 1;
				}
				QString daysforfebruary;
				daysforfebruary.setNum(daystoplusonefebruary);
				showdeadline = isyears + ismonths + daysforfebruary + " Day(s) ";
			}
			if (thismonthint == 4 || thismonthint == 6 || thismonthint == 9 || thismonthint == 11)
			{
				int daystoplusthirty;
				daystoplusthirty = daysremaining + 30;
				QString daysforothers;
				daysforothers.setNum(daystoplusthirty);
				showdeadline = isyears  + ismonths + daysforothers + " Day(s) ";
			}
		}
	}
	else if (yearsremaining > 0 && monthsremaining < 0)
	{
		editDate = targetdate;
		QString yearstominusone;
		yearstominusone.setNum(yearsremaining-1);
		QString isyears = yearstominusone + " Year(s), ";
		if (daysremaining==0)
		{
			QString monthstoplustwelve;
			monthstoplustwelve.setNum(monthsremaining+12);
			if (yearsremaining-1 == 0)
			{
				isyears="";
			}
			showdeadline= isyears + monthstoplustwelve + " Month(s) ";
		}
		if (daysremaining < 0)
		{
			QString monthstopluseleven;
			monthstopluseleven.setNum(monthsremaining+11);
			if (yearsremaining-1 == 0)
			{
				isyears="";
			}
			QString ismonths = monthstopluseleven + " Month(s), ";
			if (thismonthint == 1 || thismonthint == 3 || thismonthint == 5 || thismonthint == 7 || thismonthint == 8 || thismonthint == 10 ||thismonthint == 12)
			{
				QString daystoplusthirtyone;
				daystoplusthirtyone.setNum(daysremaining+31);
				showdeadline = isyears + ismonths + daystoplusthirtyone + " Day(s) ";
			}
			if (thismonthint == 2)
			{
				int daystoplusonefebruary;
				daystoplusonefebruary = daysremaining + 28;
				if (thisyearint%4==0)
				{
					daystoplusonefebruary = daystoplusonefebruary + 1;
				}
				QString daysforfebruary;
				daysforfebruary.setNum(daystoplusonefebruary);
				showdeadline = isyears + ismonths + daysforfebruary + " Day(s) ";
			}
			if (thismonthint == 4 || thismonthint == 6 || thismonthint == 9 || thismonthint == 11)
			{
				int daystoplusthirty;
				daystoplusthirty = daysremaining + 30;
				QString daysforothers;
				daysforothers.setNum(daystoplusthirty);
				showdeadline = isyears  + ismonths + daysforothers + " Day(s) ";
			}
		}
		if (daysremaining > 0)
		{
			int yearstominusone = yearsremaining - 1;
			QString monthstoplustwelve;
			monthstoplustwelve.setNum(monthsremaining+12);
			if (yearstominusone == 0)
			{
				showdeadline = monthstoplustwelve + " Month(s), " + daysto + " Day(s) ";
			}
			if (yearstominusone > 0)
			{
				showdeadline = yearstominusone + " Year(s), " + monthstoplustwelve + " Month(s), " + daysto +" Day(s) ";
			}
		}
	}
	QString remain;
	if (showdeadline == "") remain = "";
	else remain = "to deadline. ";
	int percent;
	QString percenttext;
	QString statsLabelStr;
	QString statsLabelToolTip;
	QDateTime now = QDateTime::currentDateTime();
	QString clock = now.toString("hh:mm");

	//Compute words
	if (!wordCountChanged) return;
	const int words = getWordCount();
	if (isPageCount)
	{
		float pageC = ((words/pageCountFormula)+1);
		pageCount = (int)pageC;
		pageCountText = pageCountText.setNum(pageCount);
		pageText = pageCountText + " pages   ";
	}
	else
	{
		pageText = "";
	}
	if (wordcount == 0)
	{
			target = " words.   " + pageText;
	}
	else if (words < wordcount || words > wordcount)
	{
		float f = words*100/wordcount;
		percent = (int)f;
		percenttext = percenttext.setNum(percent);
		target = " of " + wordcounttext + " words  (%" + percenttext + ")   " + pageText;
	}
	deadlineLabel->setText(showdeadline + remain + clock);
	statsLabel->setText(tr("%1").arg(words) + target);
	wordCountChanged = false;
}

unsigned long TextRoom::getWordCount()
{
	unsigned long words = 0;
	const QString text = textEdit->document()->toPlainText();
	bool lastWasWhitespace = false;

	for (int i = 0; i < text.count(); i++)
	{
		if (text.at(i).isSpace())
		{
			if (!lastWasWhitespace)
			{
				words++;
			}
			lastWasWhitespace = true;
		}
		else
		{
			lastWasWhitespace = false;
		}
	}

	if ((!lastWasWhitespace) && (text.count() > 0))
	{
		// one more if the last character is not a whitespace
		words++;
	}

	return words;
}

void TextRoom::documentWasModified()
{
	setWindowModified(textEdit->document()->isModified());
	
	QString text = textEdit->document()->toPlainText();
	parasnew = text.count("\n", Qt::CaseSensitive);

	if (isAutoSave && numChanges++ > 200) {
		numChanges = 0;	
		autoSave();
	} 

	if ( isFlowMode && textEdit->document()->toPlainText().size() < prevLength ) {
		textEdit->undo();
	} 

	prevLength=textEdit->document()->toPlainText().size();

	if (isSound && sdlInitialized)
	{
		if (parasnew > parasold)
		{
			if (Mix_PlayChannel(-1, soundenter, 0) == -1)
			{
				printf("Mix_PlayChannel: %s\n",Mix_GetError());
    
			}
		}
		else
		{ 
			if (Mix_PlayChannel(-1, soundany, 0) == -1)
			{
				printf("Mix_PlayChannel: %s\n",Mix_GetError());
    
			}
		}
	}

	parasold = parasnew;

	vPositionChanged();

	wordCountChanged = true;

	//getFileStatus();
}

void TextRoom::alarmTime()
{
	alarm = 0;
	QMessageBox::warning(this, qApp->applicationName(), tr("Time is out.\n"), QMessageBox::Ok);
	writeSettings();
	readSettings();
}

void TextRoom::readSettings()
{
#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	QFile file( settings.fileName() );
	if ( !file.exists() )
	{
		toggleFullScreen();
		writeSettings();
		return;
	}

	if ( settings.value("WindowState/ShowFullScreen", true).toBool() )
	{
		if ( !isFullScreen() )
			showFullScreen();
   	}
	else
	{
		showNormal();
		QPoint pos = settings.value("WindowState/TopLeftPosition", QPoint(100, 100)).toPoint();
		QSize size = settings.value("WindowState/WindowSize", QSize(300, 200)).toSize();
		resize(size);
		move(pos);
	}

	QString textColor = settings.value("Colors/FontColor", "#808080" ).toString();
	QString back = settings.value("Colors/Background", "black" ).toString();
	QString status_c = settings.value("Colors/StatusColor", "#202020" ).toString();
	QString status_b = settings.value("Colors/StatusBg", "#808080" ).toString();

	loadStyleSheet(textColor, back, status_c, status_b);

	// oxygen does weird stuff with the background
	QApplication::setStyle("plastique");

	QStringList fontS;
	QFont font;
	fontS << settings.value("Font/FileName_Settings", label->font() ).toString()
		<< settings.value("Font/Statistics_Settings", statsLabel->font() ).toString()
		<< settings.value("Font/DefaultFont", textEdit->font() ).toString();
	
	font.fromString(fontS.at(0));
	if (!(label->font() == font)) 
	{
		label->setFont( font );
		//label->setProperty("class", "mainwindow QLabel");
	}
	
	font.fromString(fontS.at(1));
	if (!(statsLabel->font() == font))
	{
		statsLabel->setFont( font );
		deadlineLabel->setFont ( font) ;
		//statsLabel->setProperty("class", "mainwindow QLabel");
	}
	
	defaultFont.fromString(fontS.at(2));
	
	curDir = settings.value("RecentFiles/LastDir", curDir).toString();
	lastSearch = settings.value("TextSearch/LastPhrase", lastSearch).toString();

	QDateTime today = QDateTime::currentDateTime();
	QString todaytext = today.toString("yyyyMMdd");
	
	isAutoSave = settings.value("AutoSave", false).toBool();
	isFlowMode = settings.value("FlowMode", false).toBool();
	isSound = settings.value("Sound", true).toBool();
	isPageCount = settings.value("PageCount", false).toBool();
	deadlinetext = settings.value("Deadline", todaytext).toString();
	deadline = QDate::fromString(deadlinetext, "yyyyMMdd");
	wordcount = settings.value("WordCount", 0).toInt();
	wordcounttext = settings.value("WordCount", 0).toString();
	editorWidth = settings.value("EditorWidth", 800).toInt();
	editorTopSpace = settings.value("EditorTopSpace", 0).toInt();
	editorBottomSpace = settings.value("EditorBottomSpace", 0).toInt();
	alarm = settings.value("TimedWriting", 0).toInt();
	pageCountFormula = settings.value("PageCountFormula", 250).toInt();
	dateFormat = settings.value("DateFormat", "dd MMMM yyyy dddd").toString();
	timeFormat = settings.value("TimeFormat", "hh:mm").toString();
	if (timeFormat == "HH:MM") timeFormat = "hh:mm"; // fix for old format

	horizontalSlider->setVisible( settings.value("ScrollBar", true).toBool() );
	isScrollBarVisible = horizontalSlider->isVisible();
	vPositionChanged();

	topSpacer->changeSize(20, editorTopSpace, QSizePolicy::Maximum, QSizePolicy::Maximum);
	bottomSpacer->changeSize(20, editorBottomSpace, QSizePolicy::Maximum, QSizePolicy::Maximum);
	
	timeOut = alarm * 60000;
	if (alarm != 0)
	{
		QTimer::singleShot(timeOut, this, SLOT(alarmTime()));
	}
	
	textEdit->setMaximumWidth(editorWidth);

	if ( (optOpenLastFile = settings.value("RecentFiles/OpenLastFile", true).toBool()) )
	{
		curFile = settings.value("RecentFiles/LastFile", curFile).toString();
		if ( (isSaveCursor = settings.value("RecentFiles/SavePosition", true).toBool() ))
			cPosition = settings.value("RecentFiles/AtPosition", cPosition).toInt();
	}

	changeDefaultFont();

	if (isSound && !sdlInitialized)	initSDLMixer();
	else if (!isSound && sdlInitialized) destroySDLMixer();
}

void TextRoom::writeSettings()
{

#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	if ( !isFullScreen() )
	{
		settings.setValue("WindowState/TopLeftPosition", pos());
		settings.setValue("WindowState/WindowSize", size());
	}

	settings.setValue("WindowState/ShowFullScreen", isFullScreen());
	settings.setValue("RecentFiles/LastFile", curFile);
	settings.setValue("RecentFiles/LastDir", curDir);
	settings.setValue("TextSearch/LastPhrase", lastSearch);
	settings.setValue("Deadline", editDate);
	settings.setValue("TimedWriting", alarm);

	int maxEditorWidth = width();
	int maxEditorTopSpace = (int)((height() - 56)/2) ;
	int maxEditorBottomSpace = (int)((height() - 56)/2);
	settings.setValue("MaxEditorWidth", maxEditorWidth);
	settings.setValue("MaxEditorTopSpace", maxEditorTopSpace);
	settings.setValue("MacEditorBottomSpace", maxEditorBottomSpace);


	settings.setValue("RecentFiles/OpenLastFile", optOpenLastFile);
	if ( optOpenLastFile )
	{
		settings.setValue("RecentFiles/SavePosition", isSaveCursor);
		if (isSaveCursor) settings.setValue("RecentFiles/AtPosition", cPosition);
	}

	settings.setValue("README","Please read the help file"
					  " by pressing F1, the help key, for"
					  " instructions on how to modify this file.");
}

void TextRoom::options()
{
	writeSettings();
	if (optionsDialog->exec() == QDialog::Accepted)
	{
		readSettings();
	}
}

void TextRoom::help()
{
	if (!helpDialog->isVisible())
	{
		int x = (width()/2)-351;
		int y = (height()/2)-150;
		helpDialog->move(x,y);
		helpDialog->setWindowFlags(Qt::FramelessWindowHint);
		helpDialog->showNormal();
	}
	else
	{
		helpDialog->hide();
	}
}

void TextRoom::loadStyleSheet(const QString &fcolor, const QString &bcolor, const QString &scolor, const QString &sbcolor)
{
	QPalette palette;

	palette.setColor(QPalette::Text, fcolor);
	palette.setColor(QPalette::Base, bcolor);
	textEdit->setPalette(palette);

	palette.setColor(QPalette::Window, bcolor);
	TextRoom::setPalette(palette);

	QPalette palette2;
	palette2.setColor(QPalette::Foreground, scolor);
	palette2.setColor(QPalette::WindowText, scolor);
	palette2.setColor(QPalette::Window, sbcolor);
	palette2.setColor(QPalette::Button, sbcolor);

	horizontalSlider->setPalette(palette2);
	label->setPalette(palette2);
	statsLabel->setPalette(palette2);
	deadlineLabel->setPalette(palette2);
	
	label->setAutoFillBackground(true);
	statsLabel->setAutoFillBackground(true);
	deadlineLabel->setAutoFillBackground(true);
}

void TextRoom::find()
{
	QString sString = SearchDialog::useSearchDialog(this, lastSearch);
	if (!sString.isEmpty())
	{
		lastSearch = sString;
		textEdit->find( sString );
		writeSettings();
	}
	
}

void TextRoom::find_next()
{
	if (!lastSearch.isEmpty())
	{
		textEdit->find( lastSearch );
	}
}

void TextRoom::find_previous()
{
	if (!lastSearch.isEmpty())
	{
		textEdit->find( lastSearch, QTextDocument::FindBackward );
	}
}

void TextRoom::sCursor()
{
	textEdit->ensureCursorVisible();
}

void TextRoom::resizeEvent(QResizeEvent *event)
{
	update();
	sCursor();
	QWidget::resizeEvent(event);

}

void TextRoom::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
	QTextCursor cursor = textEdit->textCursor();
	cursor.mergeCharFormat(format);
	textEdit->mergeCurrentCharFormat(format);
}

void TextRoom::textBold()
{
	QTextCharFormat fmt;
	QTextCursor cursor = textEdit->textCursor();
	if (cursor.charFormat().fontWeight() == 75)
	{
		fmt.setFontWeight(50);
	}
	else
	{
		fmt.setFontWeight(75);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::textItalic()
{
	QTextCharFormat fmt;
	QTextCursor cursor = textEdit->textCursor();
	if (cursor.charFormat().fontItalic())
	{
		fmt.setFontItalic(false);
	}
	else
	{
		fmt.setFontItalic(true);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::textUnderline()
{
	QTextCharFormat fmt;
	QTextCursor cursor = textEdit->textCursor();
	if (cursor.charFormat().fontUnderline())
	{
		fmt.setFontUnderline(false);
	}
	else
	{
		fmt.setFontUnderline(true);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::textSizeUp()
{
	textSize = textEdit->currentFont().pointSize();
	textSize++;
	QTextCharFormat fmt;
	fmt.setFontPointSize(textSize);
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::textSizeDown()
{
	textSize = textEdit->currentFont().pointSize();
	textSize--;
	QTextCharFormat fmt;
	fmt.setFontPointSize(textSize);
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::changeDefaultFont()
{
//	if (curFile.endsWith(".txt"))
//	{

	textEdit->setFont(defaultFont);

//	}
}

void TextRoom::changeFont()
{
	QString currentFont = textEdit->textCursor().charFormat().fontFamily();
	QString fontFamily = selectFont->useSelectFont(this);
	QTextCharFormat fmt;
	if (fontFamily != "")
	{
		fmt.setFontFamily(fontFamily);
	}
	else if (fontFamily == "")
	{
		fmt.setFontFamily(currentFont);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void TextRoom::vPositionChanged()
{
	horizontalSlider->setMinimum(textEdit->verticalScrollBar()->minimum());
	horizontalSlider->setMaximum(textEdit->verticalScrollBar()->maximum());
	if ( isScrollBarVisible )
		horizontalSlider->setVisible(textEdit->verticalScrollBar()->maximum());
	horizontalSlider->setValue( textEdit->verticalScrollBar()->value());
}

void TextRoom::hSliderPositionChanged()
{
	textEdit->verticalScrollBar()->setValue( horizontalSlider->value() );
}

void TextRoom::initSDLMixer() {
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 1024;

	if (sdlInitialized || sdlFailed)
	{
		// don't initialize again if it failed once or is still initialized
		return;
	}
	
	// assume the worst, method will return on error or set this to false at the end
	sdlFailed = true;
	
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		QMessageBox::warning(this, qApp->applicationName(),
			tr("Unable to initialize SDL:") + QString(SDL_GetError()), QMessageBox::Ok);
		return;
	}

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		QMessageBox::warning(this, qApp->applicationName(),
			tr("Unable to initialize audio:") + QString(Mix_GetError()), QMessageBox::Ok);
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return;
	}

#ifdef Q_OS_WIN32
	soundany = Mix_LoadWAV("sounds/keyany.wav");
#else
	soundany = Mix_LoadWAV("/usr/local/share/textroom/keyany.wav");
#endif
	if(soundany == NULL)
	{
		QMessageBox::warning(this, qApp->applicationName(),
			tr("Could not load WAV file:") + QString(Mix_GetError()), QMessageBox::Ok);
		destroySDLMixer();
		return;
	}

#ifdef Q_OS_WIN32
	soundenter = Mix_LoadWAV("sounds/keyenter.wav");
#else
	soundenter = Mix_LoadWAV("/usr/local/share/textroom/keyenter.wav");
#endif
	if(soundenter == NULL)
	{
		QMessageBox::warning(this, qApp->applicationName(),
			tr("Could not load WAV file:") + QString(Mix_GetError()), QMessageBox::Ok);
		destroySDLMixer();
		return;
	}

	sdlFailed = false;
	sdlInitialized = true;
}

void TextRoom::destroySDLMixer() {
	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	sdlInitialized = false;
}
