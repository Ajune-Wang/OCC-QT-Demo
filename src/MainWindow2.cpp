#include "mainwindow.h"
#include "mdichild.h"
#include "ICon.h"
#include "ComplainUtf8.h"
#include "Msg.h"
#include <QtWidgets>

void MainWindow::createActions()
{
	//�ļ�
	{
		QMenu *fileMenu = menuBar()->addMenu(tr("�ļ�&F"));

		const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
		newAct = new QAction(newIcon, tr("�½�&N"), this);
		newAct->setShortcuts(QKeySequence::New);
		newAct->setStatusTip(tr("Create a new file"));
		connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
		fileMenu->addAction(newAct);

		const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
		openAct = new QAction(openIcon, tr("��&O..."), this);
		openAct->setShortcuts(QKeySequence::Open);
		openAct->setStatusTip(tr("Open an existing file"));
		connect(openAct, &QAction::triggered, this, &MainWindow::open);
		fileMenu->addAction(openAct);

		const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
		saveAct = new QAction(saveIcon, tr("����&S"), this);
		saveAct->setShortcuts(QKeySequence::Save);
		saveAct->setStatusTip(tr("Save the document to disk"));
		connect(saveAct, &QAction::triggered, this, &MainWindow::save);
		fileMenu->addAction(saveAct);

		const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(":/images/saveas.png"));
		saveAsAct = new QAction(saveAsIcon, tr("���Ϊ&A..."), this);
		saveAsAct->setShortcuts(QKeySequence::SaveAs);
		saveAsAct->setStatusTip(tr("Save the document under a new name"));
		connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
		fileMenu->addAction(saveAsAct);
		fileMenu->addSeparator();

		closeAct = new QAction(tr("�ر�&O"), this);
		closeAct->setStatusTip(tr("Close the active window"));
		connect(closeAct, &QAction::triggered,
			mdiArea, &QMdiArea::closeActiveSubWindow);
		fileMenu->addAction(closeAct);

		closeAllAct = new QAction(tr("�ر�ȫ��&A"), this);
		closeAllAct->setStatusTip(tr("Close all the windows"));
		connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);
		fileMenu->addAction(closeAllAct);

		fileMenu->addSeparator();
		QMenu *recentMenu = fileMenu->addMenu(tr("�����&R..."));
		connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
		recentFileSubMenuAct = recentMenu->menuAction();

		for (int i = 0; i < MaxRecentFiles; ++i) {
			recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
			recentFileActs[i]->setVisible(false);
		}

		recentFileSeparator = fileMenu->addSeparator();

		setRecentFilesVisible(MainWindow::hasRecentFiles());

		//! [0]
		const QIcon exitIcon = QIcon::fromTheme("application-exit");
		QAction *exitAct = fileMenu->addAction(exitIcon, tr("�˳�&X"), qApp, &QApplication::closeAllWindows);
		exitAct->setShortcuts(QKeySequence::Quit);
		exitAct->setStatusTip(tr("Exit the application"));
		fileMenu->addAction(exitAct);
		//! [0]
	}

	//����
	{
		windowMenu = menuBar()->addMenu(tr("����&W"));
		connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

		tileAct = new QAction(tr("����&T"), this);
		tileAct->setStatusTip(tr("Tile the windows"));
		connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

		cascadeAct = new QAction(tr("����&C"), this);
		cascadeAct->setStatusTip(tr("Cascade the windows"));
		connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);

		nextAct = new QAction(tr("��һ��&X"), this);
		nextAct->setShortcuts(QKeySequence::NextChild);
		nextAct->setStatusTip(tr("Move the focus to the next window"));
		connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

		previousAct = new QAction(tr("��һ��&V"), this);
		previousAct->setShortcuts(QKeySequence::PreviousChild);
		previousAct->setStatusTip(tr("Move the focus to the previous window"));
		connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

		windowMenuSeparatorAct = new QAction(this);
		windowMenuSeparatorAct->setSeparator(true);

		updateWindowMenu();

		menuBar()->addSeparator();
	}
	//MDI�˵�
	{
		this->createMDIActions();
	}
	//����
	{
		QMenu *helpMenu = menuBar()->addMenu(tr("����&H"));
		QAction *aboutAct = helpMenu->addAction(tr("����&A"), this, &MainWindow::about);
		aboutAct->setStatusTip(tr("Show the application's About box"));
		QAction *aboutQtAct = helpMenu->addAction(tr("����&Qt"), qApp, &QApplication::aboutQt);
		aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	}
	//{//������
	//	QToolBar *fileToolBar = addToolBar(tr("File"));
	//	fileToolBar->addAction(newAct);
	//	fileToolBar->addAction(openAct);
	//	fileToolBar->addAction(saveAct);
	//}
}

void MainWindow::createMDIActions()
{
	//��ͼ
	{
		Mdi_ViewMenu = menuBar()->addMenu("��ͼ&V");
	}
	//ģ��
	{
		QMenu *  Mdi_ModelMenu = menuBar()->addMenu("ģ��&M");
		ChildMenus += Mdi_ModelMenu;

		{
			QAction * ImportModelAct = new QAction(tr("����ģ��&I"), this);
			ImportModelAct->setStatusTip(tr("����ģ�͵�OCCϵͳ"));
			connect(ImportModelAct, &QAction::triggered, this, [=]()
			{
				if (activeMdiChild())
				{
					activeMdiChild()->ImportModel();
					statusBar()->showMessage(tr("����ģ��ing"), 2000);
				}
			});
			Mdi_ModelMenu->addAction(ImportModelAct);
		}
		{
			QAction * ExportModelAct = new QAction(tr("����ģ��&E"), this);
			ExportModelAct->setStatusTip(tr("����ģ�͵��ļ�"));
			connect(ExportModelAct, &QAction::triggered, this, [=]()
			{
				if (activeMdiChild())
				{
					activeMdiChild()->ExportModel();
					statusBar()->showMessage(tr("����ģ��ing"), 2000);
				}
			});
			Mdi_ModelMenu->addAction(ExportModelAct);
		}

		//QAction * ImportModelAssAct = new QAction(tr("����ģ��_װ����&I"), this);
		//ImportModelAssAct->setStatusTip(tr("����ģ��_װ���嵽MBD��ע"));
		//connect(ImportModelAssAct, &QAction::triggered, this, [=]() {
		//	if (activeMdiChild())
		//	{
		//		//activeMdiChild()->document()->ImportModelAss();
		//		statusBar()->showMessage(tr("����ģ��_װ����ing"), 2000);
		//	}
		//});
		//Mdi_ModelMenu->addAction(ImportModelAssAct);
	}
	//ѡ��
	{
		QMenu * Mdi_SelMenu = menuBar()->addMenu("ѡ��&S");
		ChildMenus += Mdi_SelMenu;
		QAction * SelNaturalAct = new QAction(QIcon(ICON_SEL_SHAPE), tr("��Ȼ&N"), this);
		SelNaturalAct->setStatusTip(tr("ѡ����Ȼģ��"));
		connect(SelNaturalAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelNatural();
		});
		Mdi_SelMenu->addAction(SelNaturalAct);
		QAction * SelSolidAct = new QAction(QIcon(ICON_SEL_SOLID), tr("��&S"), this);
		SelSolidAct->setStatusTip(tr("ѡ��ģ�͡�����"));
		connect(SelSolidAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelSolid();
		});
		Mdi_SelMenu->addAction(SelSolidAct);
		QAction * SelFaceAct = new QAction(QIcon(ICON_SEL_FACE), tr("��&F"), this);
		SelFaceAct->setStatusTip(tr("ѡ��ģ�͡�����"));
		connect(SelFaceAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelFace();
		});
		Mdi_SelMenu->addAction(SelFaceAct);
		QAction * SelEdgeAct = new QAction(QIcon(ICON_SEL_EDGE), tr("��&E"), this);
		SelEdgeAct->setStatusTip(tr("ѡ��ģ�͡�����"));
		connect(SelEdgeAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelEdge();
		});
		Mdi_SelMenu->addAction(SelEdgeAct);
		QAction * SelVertexAct = new QAction(QIcon(ICON_SEL_VERTEX), tr("��&E"), this);
		SelVertexAct->setStatusTip(tr("ѡ��ģ�͡�����"));
		connect(SelVertexAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelVertex();
		});
		Mdi_SelMenu->addAction(SelVertexAct);
		QToolBar * Mdi_SelToolBar = this->addToolBar("����ѡ�񹤾���");
		ChildToolBars += Mdi_SelToolBar;
		Mdi_SelToolBar->addAction(SelNaturalAct);
		Mdi_SelToolBar->addAction(SelSolidAct);
		Mdi_SelToolBar->addAction(SelFaceAct);
		Mdi_SelToolBar->addAction(SelEdgeAct);
		Mdi_SelToolBar->addAction(SelVertexAct);
	}
	//AIS
	{
		QMenu * Mdi_AISMenu = menuBar()->addMenu("��ʾ&S");
		ChildMenus += Mdi_AISMenu;
		QAction * DisplayAllAct = new QAction(QIcon(ICON_AIS_DISPLAYALL), tr("��ʾ����&D"), this);
		DisplayAllAct->setStatusTip(tr("��ʾ����"));
		connect(DisplayAllAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjDisplayAll();
		});
		Mdi_AISMenu->addAction(DisplayAllAct);
		QAction * EraseAllAct = new QAction(QIcon(ICON_AIS_ERASEALL), tr("��������&E"), this);
		EraseAllAct->setStatusTip(tr("��������"));
		connect(EraseAllAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjEraseAll();
		});
		Mdi_AISMenu->addAction(EraseAllAct);
		QAction * SetColorAct = new QAction(QIcon(ICON_AIS_OBJCOLOR), tr("���ö�����ɫ&C"), this);
		SetColorAct->setStatusTip(tr("���ö�����ɫ"));
		connect(SetColorAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelColor();
		});
		Mdi_AISMenu->addAction(SetColorAct);
		QAction * SetTransparencyAct = new QAction(QIcon(ICON_AIS_TRANSPARENCY), tr("����͸����&T"), this);
		SetTransparencyAct->setStatusTip(tr("����͸����"));
		connect(SetTransparencyAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelTrans();
		});
		Mdi_AISMenu->addAction(SetTransparencyAct);
		QAction * HideAct = new QAction(QIcon(ICON_AIS_ERASE), tr("����&H"), this);
		HideAct->setStatusTip(tr("����"));
		connect(HideAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjHide();
		});
		Mdi_AISMenu->addAction(HideAct);
		//QAction * SelColorAct = new QAction(QIcon(ICON_AIS_SELCOLOR), tr("����ѡ�������ɫ&S"), this);
		//SelColorAct->setStatusTip(tr("����ѡ�������ɫ"));
		//connect(SelColorAct, &QAction::triggered, this, [=]() {
		//	if (activeMdiChild())activeMdiChild()->SelColor();
		//});
		//Mdi_AISMenu->addAction(SelColorAct);
		QToolBar * Mdi_AISToolBar = this->addToolBar("��ʾ������");
		ChildToolBars += Mdi_AISToolBar;
		Mdi_AISToolBar->addAction(DisplayAllAct);
		Mdi_AISToolBar->addAction(EraseAllAct);
		Mdi_AISToolBar->addAction(SetColorAct);
		Mdi_AISToolBar->addAction(SetTransparencyAct);
		Mdi_AISToolBar->addAction(HideAct);
		//Mdi_AISToolBar->addAction(SelColorAct);
	}
	//����
	{
		QMenu * Mdi_TestMenu = menuBar()->addMenu("����&T");
		ChildMenus += Mdi_TestMenu;
		{
			QAction * TestActT = new QAction(tr("����-����1"), this);
			connect(TestActT, &QAction::triggered, this, [=]() {
				if (activeMdiChild())
				{
					activeMdiChild()->Test1();
					statusBar()->showMessage(tr("����-����1"), 2000);
				}
			});
			Mdi_TestMenu->addAction(TestActT);
		}
	}
}
