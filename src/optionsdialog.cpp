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

#include "optionsdialog.h"
#include "textroom.h"

OptionsDialog::OptionsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	reaSettings();

	connect(ui.pushButton, SIGNAL( clicked() ), this, SLOT( startAlarm() ) );
}

void OptionsDialog::startAlarm()
{
	if (ui.spinBox->value() > 0)
	{
	setAlarm = ui.spinBox->value();
	writSettings();
	close();	
	}
	else
	{
	setAlarm = 0;
	writSettings();
	}
}

void OptionsDialog::reaSettings()
{
#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	QDateTime today = QDateTime::currentDateTime();
	QString todaytext = today.toString("yyyyMMdd");
	QStringList fontS;
	QFont font;
	QFont defaultFont;

	fontS << settings.value("Font/FileName_Settings", ui.statusbarFontComboBox->currentFont() ).toString()
		<< settings.value("Font/Statistics_Settings", ui.statusbarFontComboBox->currentFont() ).toString()
		<< settings.value("Font/DefaultFont", ui.fontComboBox->currentFont() ).toString();
	
	font.fromString(fontS.at(0));
	ui.statusbarFontComboBox->setCurrentFont( font );
	ui.statusbarBoldCheckBox->setChecked( font.bold() );
	ui.statusbarItalicCheckBox->setChecked( font.italic() );
	ui.statusbarSpinBox->setValue( font.pointSize() );
	
	defaultFont.fromString(fontS.at(2));
	ui.fontComboBox->setCurrentFont( defaultFont );
	ui.fontSizeSpinBox->setValue( defaultFont.pointSize() );
	
	ui.loadOnStartCheckBox->setChecked( settings.value( "RecentFiles/OpenLastFile", true ).toBool() );
	ui.saveCursorCheckBox->setChecked( settings.value( "RecentFiles/SavePosition", true ).toBool() );
	if ( !ui.loadOnStartCheckBox->isChecked() )
		ui.saveCursorCheckBox->setEnabled( false );
	
	ui.editorWidthSpinBox->setMaximum( settings.value("MaxEditorWidth", 1024).toInt());
	ui.editorTopSpaceSpinBox->setMaximum(settings.value("MaxEditorTopSpace", 768).toInt());
	ui.editorBottomSpaceSpinBox->setMaximum(settings.value("MaxEditorBottomSpace", 1000).toInt());
	ui.wordCountSpinBox->setValue( settings.value("WordCount", 0).toInt());
	ui.pageCountSpinBox->setValue( settings.value("PageCountFormula", 250).toInt());
	ui.fullScreenCheckBox->setChecked( settings.value("WindowState/ShowFullScreen", true).toBool() );
	ui.splashScreenCheckBox->setChecked( settings.value("WindowState/ShowSplashScreen", true).toBool() );
	ui.autoSaveCheckBox->setChecked( settings.value("AutoSave", false).toBool() );
	ui.flowModeCheckBox->setChecked( settings.value("FlowMode", false).toBool() );
	ui.scrollBarCheckBox->setChecked( settings.value("ScrollBar", true).toBool() );
	ui.pageCountCheckBox->setChecked( settings.value("PageCount", false).toBool() );
	ui.soundCheckBox->setChecked( settings.value("Sound", true).toBool() );
	QString datetext = settings.value("Deadline", todaytext).toString();
	QDate date;
	QDate dateselected = date.fromString(datetext, "yyyyMMdd");
	ui.calendarWidget->setSelectedDate(dateselected);
	ui.editorWidthSpinBox->setValue( settings.value	("EditorWidth", 800).toInt());  
	ui.editorTopSpaceSpinBox->setValue( settings.value("EditorTopSpace", 0).toInt());
	ui.editorBottomSpaceSpinBox->setValue( settings.value("EditorBottomSpace", 0).toInt());
	ui.spinBox->setValue( settings.value("TimedWriting", 0 ).toInt());
	ui.dateFormat->setText( settings.value("DateFormat", "dd MMMM yyyy dddd").toString());
	ui.timeFormat->setText( settings.value("TimeFormat", "HH:MM").toString());
	
	QPalette palette;
	
	palette.setColor(ui.pbFontColor->backgroundRole(),
		fcolor = settings.value("Colors/FontColor", "#808080" ).toString());
	ui.pbFontColor->setPalette(palette);	

	palette.setColor(ui.pbStatusBarColor->backgroundRole(),
		scolor = settings.value("Colors/StatusColor", "#202020" ).toString());
	ui.pbStatusBarColor->setPalette(palette);

	
	palette.setColor(ui.pbEditorBackColor->backgroundRole(),
		bgcolor = settings.value("Colors/Background", "black" ).toString());
	ui.pbEditorBackColor->setPalette(palette);
	
	palette.setColor(ui.pbStatusBarBgColor->backgroundRole(),
		sbcolor = settings.value("Colors/StatusBg", "#808080").toString());
	ui.pbStatusBarBgColor->setPalette(palette);


}

void OptionsDialog::writSettings()
{

#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	settings.setValue("Colors/FontColor", fcolor.name() );
	settings.setValue("Colors/Background", bgcolor.name() );
	settings.setValue("Colors/StatusColor", scolor.name() );
	settings.setValue("Colors/StatusBg", sbcolor.name() );

	settings.setValue("RecentFiles/OpenLastFile", ui.loadOnStartCheckBox->isChecked() );
	settings.setValue("RecentFiles/SavePosition", ui.saveCursorCheckBox->isChecked() );
	settings.setValue("ScrollBar", ui.scrollBarCheckBox->isChecked() );

	settings.setValue("WindowState/ShowFullScreen", ui.fullScreenCheckBox->isChecked() );
	settings.setValue("WindowState/ShowSplashScreen", ui.splashScreenCheckBox->isChecked() );
	settings.setValue("Sound", ui.soundCheckBox->isChecked() );
	settings.setValue("AutoSave", ui.autoSaveCheckBox->isChecked() );
	settings.setValue("FlowMode", ui.flowModeCheckBox->isChecked() );
	settings.setValue("PageCount", ui.pageCountCheckBox->isChecked() );
	settings.setValue("WordCount", ui.wordCountSpinBox->value() );
	settings.setValue("Deadline", ui.calendarWidget->selectedDate().toString("yyyyMMdd"));
	settings.setValue("EditorWidth", ui.editorWidthSpinBox->value() );
	settings.setValue("TimedWriting", setAlarm );
	settings.setValue("EditorTopSpace", ui.editorTopSpaceSpinBox->value() );
	settings.setValue("EditorBottomSpace", ui.editorBottomSpaceSpinBox->value() );
	settings.setValue("PageCountFormula", ui.pageCountSpinBox->value() );
	settings.setValue("DateFormat", ui.dateFormat->text() );
	settings.setValue("TimeFormat", ui.timeFormat->text() );

	QFont font;
	QFont defaultFont;
	
	font = ui.statusbarFontComboBox->currentFont();
	font.setBold(ui.statusbarBoldCheckBox->isChecked() );
	font.setItalic(ui.statusbarItalicCheckBox->isChecked() );
	font.setPointSize(ui.statusbarSpinBox->value() );
	settings.setValue("Font/FileName_Settings", font.toString() );
	
	font = ui.statusbarFontComboBox->currentFont();
	font.setBold(ui.statusbarBoldCheckBox->isChecked() );
	font.setItalic(ui.statusbarItalicCheckBox->isChecked() );
	font.setPointSize(ui.statusbarSpinBox->value() );
	settings.setValue("Font/Statistics_Settings", font.toString() );

	defaultFont = ui.fontComboBox->currentFont();
	defaultFont.setPointSize(ui.fontSizeSpinBox->value() );
	settings.setValue("Font/DefaultFont", defaultFont.toString() );
   	
}

void OptionsDialog::on_pushButtonOk_clicked()
{
	writSettings();
	accept();
}

void OptionsDialog::showEvent( QShowEvent * )
{
	reaSettings();
}

//STATUS BACKGROUND
void OptionsDialog::on_pbStatusBarBgColor_clicked()
{
	showStatusBarBgColorDialog();
}

void OptionsDialog::showStatusBarBgColorDialog()
{
	QColor c = QColorDialog::getColor(sbcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbStatusBarBgColor->backgroundRole(), sbcolor = c);
		ui.pbStatusBarBgColor->setPalette(palette);
		ui.pbStatusBarBgColor->setAutoFillBackground(true);
	}
}


// BACKGROUND

void OptionsDialog::on_pbEditorBackColor_clicked()
{
	showBackgroundDialog();
}

void OptionsDialog::showBackgroundDialog()
{
	QColor c = QColorDialog::getColor(bgcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbEditorBackColor->backgroundRole(), bgcolor = c);
		ui.pbEditorBackColor->setPalette(palette);
		ui.pbEditorBackColor->setAutoFillBackground(true);
	}
}

// STATUSBARTEXTCOLOR

void OptionsDialog::on_pbStatusBarColor_clicked()
{
	showStatusColorDialog();
}

void OptionsDialog::showStatusColorDialog()
{
	QColor c = QColorDialog::getColor(scolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbStatusBarColor->backgroundRole(), scolor = c);
		ui.pbStatusBarColor->setPalette(palette);
		ui.pbStatusBarColor->setAutoFillBackground(false);
	}
}

// TEXTCOLOR

void OptionsDialog::on_pbFontColor_clicked()
{
	showFontColorDialog();
}

void OptionsDialog::showFontColorDialog()
{
	QColor c = QColorDialog::getColor(fcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbFontColor->backgroundRole(), fcolor = c);
		ui.pbFontColor->setPalette(palette);
		ui.pbFontColor->setAutoFillBackground(false);
	}
}
