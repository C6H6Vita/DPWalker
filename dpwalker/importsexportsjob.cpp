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
#include "importsexportsjob.h"

#include <QRegExp>
#include <QTreeWidget>


QIcon iconFromType( const QString &type )
{
	if ( type == "F" )
	{
		return QIcon( ":/icons/function.png" );
	}

	if ( type == "O" )
	{
		return QIcon( ":/icons/object.png" );
	}

	return QIcon();
}


ExportsJob::ExportsJob( const QString &file, QTreeWidget *pExports, QMap<QString, QString> *pMap ) :
	m_file( file ),
	m_pExports( pExports ),
	m_pExportsMap( pMap ),
	m_proc(),
	m_stream( &m_proc )
{
	m_proc.setReadChannel( QProcess::StandardOutput );

	connect( &m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readLineStdout()) );
	connect( &m_proc, SIGNAL(finished(int)), this, SLOT(readLineStdout()) );
}

void ExportsJob::start()
{
    const QStringList args = QStringList() << "-TC" << m_file;

	m_proc.start( "objdump", args );
}


void ExportsJob::readLineStdout()
{
	while ( m_proc.canReadLine() || !m_stream.atEnd() )
	{
		const QString line = m_stream.readLine();

		const QRegExp re( "^([0-9a-f]+)\\s(.)(.)(.)(.)(.)(.)(.)\\s(\\S+)\t([0-9a-f]+)\\s\\s?(\\s{11}|\\S+\\s*)\\s(\\S.*)$" );
		if ( re.indexIn( line ) >= 0 )
		{
            if ( re.cap( 9 ).compare( "*UND*" ) == 0 )
                continue;

			QTreeWidgetItem *pItem = new QTreeWidgetItem( m_pExports );

			pItem->setText( 0, re.cap( 12 ) );
			pItem->setText( 1, re.cap( 1 ) );
			pItem->setText( 2, re.cap( 2 ) );
			pItem->setText( 3, re.cap( 3 ) );
			pItem->setText( 4, re.cap( 4 ) );
			pItem->setText( 5, re.cap( 5 ) );
			pItem->setText( 6, re.cap( 6 ) );
			pItem->setText( 7, re.cap( 7 ) );
			pItem->setText( 8, re.cap( 9 ) );
			pItem->setText( 9, re.cap( 10 ) );
			pItem->setText( 10, re.cap( 11 ) );

			const QIcon icon = iconFromType( re.cap( 8 ) );
			pItem->setIcon( 0, icon );

			(*m_pExportsMap)[ re.cap( 12 ) ] = line;
		}
	}

	if ( m_proc.state() == QProcess::NotRunning )
	{
		emit finished();
	}
}


ImportsJob::ImportsJob( const QString &file, QTreeWidget *pImports, const QMap<QString, QString> *pMap ) :
	m_file( file ),
	m_pImports( pImports ),
	m_pImportsMap( pMap ),
	m_proc(),
	m_stream( &m_proc )
{
	m_proc.setReadChannel( QProcess::StandardOutput );

	connect( &m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readLineStdout()) );
	connect( &m_proc, SIGNAL(finished(int)), this, SLOT(readLineStdout()) );
}


void ImportsJob::start()
{
	const QStringList args = QStringList() << "-TC" << m_file;

	m_proc.start( "objdump", args );
}


void ImportsJob::readLineStdout()
{
	while ( m_proc.canReadLine() || !m_stream.atEnd() )
	{
		const QString line = m_stream.readLine();

		const QRegExp re( "^[0-9a-f]+\\s......(.)\\s(\\S+)\t[0-9a-f]+\\s\\s?(\\s{11}|\\S+\\s*)\\s(\\S.*)$" );
		if ( re.indexIn( line ) >= 0 )
		{
			if ( re.cap( 2 ).compare( "*UND*" ) != 0 )
				continue;

			if ( m_pImportsMap->contains( re.cap( 4 ) ) )
			{
				QTreeWidgetItem *pItem = new QTreeWidgetItem( m_pImports );

				pItem->setText( 0, re.cap( 4 ) );

				const QIcon icon = iconFromType( re.cap( 1 ) );
				pItem->setIcon( 0, icon );
			}
		}
	}

	if ( m_proc.state() == QProcess::NotRunning )
	{
		emit finished();
	}
}

ImportsExportsJob::ImportsExportsJob( QTreeWidgetItem *pItem, QTreeWidget *pImports, QTreeWidget *pExports ) :
	m_pItem( pItem ),
	m_exportsJob( pItem->text( 1 ), pExports, &m_map ),
	m_importsJob( pItem->parent() ? pItem->parent()->text( 1 ) : "", pImports, &m_map )
{
	pImports->clear();
	pExports->clear();

	connect( &m_exportsJob, SIGNAL(finished()), &m_importsJob, SLOT(start()) );
	connect( &m_importsJob, SIGNAL(finished()), this, SIGNAL(finished()) );

	m_exportsJob.start();
}
