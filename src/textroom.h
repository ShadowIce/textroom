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

#ifndef TEXTROOM_H
#define TEXTROOM_H

#include <QtGui>
#include <SDL/SDL_mixer.h>

#include "ui_textroom.h"

class QShortcut;
class QFileSystemWatcher;
class OptionsDialog;
class HelpDialog;
class SearchDialog;
class SelectFont;

class TextRoom : public QWidget, public Ui::TextRoomMain
{
	Q_OBJECT
 
public:
	TextRoom(QWidget *parent = 0, Qt::WFlags f = 0 );
	~TextRoom();

private slots:

	void open();
	bool save();
	bool saveAs();
	void autoSave();
	void about();
	void options();
	void newFile();
	void help();
	void insertDate();
	void insertTime();
	void find();
	void find_next();
	void find_previous();
	void sCursor();
	void toggleFullScreen();
	void toggleEscape();
	void documentWasModified();	
	void getFileStatus();
	void readSettings();
	void alarmTime();
	void loadStyleSheet(const QString &fcolor, const QString &bcolor, const QString &scolor, const QString &sbcolor);
	void textBold();
	void textUnderline();
	void textItalic();
	void textSizeUp();
	void textSizeDown();
	void changeFont();
	void vPositionChanged();
	void hSliderPositionChanged();
	void changeDefaultFont();
	 
private:
	bool maybeSave();
	QString strippedName(const QString &fullFileName);
	void clearFormating(const QTextBlock& block);	
	void writeSettings();
	bool saveFile(const QString &fileName);
	void loadFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
	void initSDLMixer();
	void destroySDLMixer();
	unsigned long getWordCount();
	void updateLabel();

	HelpDialog *helpDialog;
	OptionsDialog *optionsDialog;
	SearchDialog *searchDialog;
	SelectFont *selectFont;
	Ui::TextRoomMain ui;
	QString curFile;
	QString curDir;
	QString lastSearch;
	QString text;
	QString deadlinetext;
	QDate deadline;
	QDate today;
	QString dateFormat;
	QString timeFormat;
	int wordcount;
	int parasold;
	int parasnew;
	QString wordcounttext;
	QFont defaultFont;
	int alarm;
	int timeOut;
	int sentenceTally;
	QFileSystemWatcher* fw;
	bool optOpenLastFile;
	bool isSaveCursor;
	int editorWidth;
	int editorTopSpace;
	int editorBottomSpace;
	int numChanges;
	int prevLength;
	int cPosition;
	int textSize;
	int pageCount;
	QString pageCountText;
	QString pageText;
	int pageCountFormula;
	bool ind;
	bool isAutoSave;
	bool isFlowMode;
	bool isSound;
	bool isIndent;
	bool isScrollBarVisible;
	bool isPageCount;
	QString filename1;
	QString filename2;
	QByteArray ba1;
	QByteArray ba2;
	const char *filename1_char;
	const char *filename2_char;
	int channel;
	bool sdlInitialized;
	bool sdlFailed;
	Mix_Chunk *soundenter;
	Mix_Chunk *soundany;
	bool wordCountChanged;

protected:
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);	

};
 
 
#endif
