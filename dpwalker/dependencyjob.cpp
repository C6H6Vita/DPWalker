/***************************************************************************
 *   Copyright (C) 2007 by Bernhard Beschow                                *
 *   bbeschow (.at) cs.tu-berlin.de                                        *
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
#include "dependencyjob.h"

#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>
#include <QTreeWidgetItem>

LibraryPathsJob::LibraryPathsJob( const QString &file, QMap<QString, QString> *libraryPaths, QObject *parent ) :
	QObject( parent ),
	m_file( file ),
	m_libraryPaths( libraryPaths ),
	m_proc(),
	m_stream( &m_proc )
{
	m_proc.setReadChannel( QProcess::StandardOutput );

	connect( &m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readLineStdout()) );
	connect( &m_proc, SIGNAL(finished(int)), this, SLOT(readLineStdout()) );
}


void LibraryPathsJob::start()
{
	const QStringList args = QStringList() << m_file;

	m_proc.start( "ldd", args );
}


void LibraryPathsJob::readLineStdout()
{
	while ( m_proc.canReadLine() || !m_stream.atEnd() )
	{
		const QString line = m_stream.readLine();

		const QRegExp re( "^\\s+(\\S+) => (\\S+) \\(0x[0-9a-f]+\\)" );
		if ( re.indexIn( line ) >= 0 )
		{
			(*m_libraryPaths)[ re.cap( 1 ) ] = re.cap( 2 );
		}
	}

	if ( m_proc.state() == QProcess::NotRunning )
	{
		emit finished();
	}
}


DependenciesJob::DependenciesJob( QTreeWidgetItem *treeWidgetItem, const QMap<QString, QString> *libraryPaths, QObject *parent ) :
	QObject( parent ),
	m_treeWidgetItem( treeWidgetItem ),
	m_libraryPaths( libraryPaths ),
	m_proc(),
	m_stream( &m_proc )
{
	m_proc.setReadChannel( QProcess::StandardOutput );

	connect( &m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readLineStdout()) );
	connect( &m_proc, SIGNAL(finished(int)), this, SLOT(readLineStdout()) );
}


void DependenciesJob::start()
{
	const QStringList args = QStringList() << "-p" << m_treeWidgetItem->text( 1 );

	m_proc.start( "objdump", args );
}


void DependenciesJob::readLineStdout()
{
	while ( m_proc.canReadLine() || !m_stream.atEnd() )
	{
		const QString line = m_stream.readLine();

		const QRegExp re( "\\s+NEEDED\\s+(\\S+)" );
		if ( re.indexIn( line ) >= 0 )
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem( m_treeWidgetItem );

			pItem->setText( 0, re.cap( 1 ) );
			pItem->setText( 1, (*m_libraryPaths)[ re.cap( 1 ) ] );
		}
	}

	if ( m_proc.state() == QProcess::NotRunning )
	{
		emit finished();
	}
}


DependencyJobs::DependencyJobs( QTreeWidgetItem *treeWidgetItem, QObject *parent ) :
	QObject( parent ),
	m_libraryPaths(),
	m_libraryPathsJob( treeWidgetItem->text( 1 ), &m_libraryPaths, this ),
	m_dependenciesJob( treeWidgetItem, &m_libraryPaths, this )
{
	connect( &m_libraryPathsJob, SIGNAL(finished()), &m_dependenciesJob, SLOT(start()) );
	connect( &m_dependenciesJob, SIGNAL(finished()), this, SIGNAL(finished()) );

	m_libraryPathsJob.start();
}
