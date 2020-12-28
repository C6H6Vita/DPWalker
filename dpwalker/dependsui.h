/***************************************************************************
 *   Copyright (C) 2007 by Bernhard Beschow                                *
 *   bbeschow (.at) cs.tu-berlin.de                                        *
 *   Copyright (C) 2020 by C6H6Vita                                        *
 *   c6h6 (.at) outlook.kr                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef DEPENDSUI_H
#define DEPENDSUI_H

#include "ui_dependsuibase.h"

#include <QMainWindow>

class QTreeWidgetItem;

class DependsUI : public QMainWindow
{
Q_OBJECT

public:
	DependsUI( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
	~DependsUI();
	/*$PUBLIC_FUNCTIONS$*/

	void openFile( const QString &file );

public slots:
	/*$PUBLIC_SLOTS$*/
	virtual void fileExit();
	virtual void fileOpen();

private slots:
	void dependencyExpanded( QTreeWidgetItem *pItem );
	void dependencySelected( QTreeWidgetItem *pItem );
	void setRootItemExpanded();

private:
	Ui::DependsUIBase m_ui;
	QString m_dirPath;
};

#endif

