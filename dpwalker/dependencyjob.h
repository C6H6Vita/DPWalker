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
#ifndef DEPENDENCYJOB_H
#define DEPENDENCYJOB_H

#include <qobject.h>

#include <QMap>
#include <QProcess>
#include <QTextStream>

class QTreeWidgetItem;

class LibraryPathsJob : public QObject
{
	Q_OBJECT
public:
	LibraryPathsJob( const QString &file, QMap<QString, QString> *libraryPaths, QObject *parent = 0 );

signals:
	void finished();

public slots:
	void start();

protected slots:
	void readLineStdout();

protected:
	QString m_file;
	QMap<QString, QString> *const m_libraryPaths;
	QProcess m_proc;
	QTextStream m_stream;
};

class DependenciesJob : public QObject
{
	Q_OBJECT
public:
	DependenciesJob( QTreeWidgetItem *treeWidgetItem, const QMap<QString, QString> *libraryPaths, QObject *parent = 0 );

signals:
	void finished();

public slots:
	void start();

protected slots:
	void readLineStdout();

protected:
	QTreeWidgetItem *const m_treeWidgetItem;
	const QMap<QString, QString> *m_libraryPaths;
	QProcess m_proc;
	QTextStream m_stream;
};

/**
 * @author Bernhard Beschow <bbeschow (.at) cs.tu-berlin.de>
 */
class DependencyJobs : public QObject
{
	Q_OBJECT
public:
	DependencyJobs( QTreeWidgetItem *treeWidgetItem, QObject *parent = 0 );

signals:
	void finished();

protected:
	QMap<QString, QString> m_libraryPaths;
	LibraryPathsJob m_libraryPathsJob;
	DependenciesJob m_dependenciesJob;
};

#endif
