#include "OCCT_GraphDriver.h"
#include "ComplainUtf8.h"
#include <OpenGl_GraphicDriver.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Axis2Placement.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <AIS_DisplayMode.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_Point.hxx>
#include <AIS_Point.hxx>

const Handle(OpenGl_GraphicDriver) & GetGraphicDriver()
{
	static Handle(OpenGl_GraphicDriver)     H_GraphicDriver = NULL;
	if (H_GraphicDriver.IsNull())
	{
		//================================OPENGL�����豸============================
		try
		{
			Handle(Aspect_DisplayConnection) theDisp = new Aspect_DisplayConnection();
			H_GraphicDriver = new OpenGl_GraphicDriver(theDisp);
		}
		catch (Standard_Failure)
		{
			ExitProcess(1);
		}
		//================================OPENGL�����豸============================
	}

	return H_GraphicDriver;
}

Handle(V3d_Viewer) OCCT_GraphDriver::CreateViewer()
{
	const Handle(OpenGl_GraphicDriver) & hGraphicDriver = ::GetGraphicDriver();
	const Handle(V3d_Viewer) & hViewer = new V3d_Viewer(hGraphicDriver);
	hViewer->SetDefaultLights();
	return hViewer;
}

Handle(AIS_InteractiveContext) OCCT_GraphDriver::CreateAISContext(const Handle(V3d_Viewer) & hViewer)
{
	Handle(AIS_InteractiveContext) hAISContext =  new AIS_InteractiveContext(hViewer); //��������ʽ����
	hAISContext->SetDisplayMode(AIS_Shaded, true);   //������ʾģʽΪ�ڱ�
	return hAISContext;
}

Handle(AIS_InteractiveContext) OCCT_GraphDriver::CreateViewCubeAISContext(const Handle(V3d_Viewer) & hViewer)
{
	Handle(AIS_InteractiveContext) hAISContext = new AIS_InteractiveContext(hViewer); //��������ʽ����
	hAISContext->SetDisplayMode(AIS_Shaded, true);   //������ʾģʽΪ�ڱ�
	{
		Handle(AIS_ViewCube) H_AisViewCube = new AIS_ViewCube();
		H_AisViewCube->SetBoxSideLabel(V3d_Xpos, "����ͼ");//!< (+Y+Z) view
		H_AisViewCube->SetBoxSideLabel(V3d_Ypos, "����ͼ");//!< (+X+Z) view
		H_AisViewCube->SetBoxSideLabel(V3d_Zpos, "����ͼ");//!< (+Y+Z) view
		H_AisViewCube->SetBoxSideLabel(V3d_Xneg, "����ͼ");//!< (+Y+Z) view
		H_AisViewCube->SetBoxSideLabel(V3d_Yneg, "����ͼ");//!< (+Y+Z) view
		H_AisViewCube->SetBoxSideLabel(V3d_Zneg, "����ͼ");//!< (+Y+Z) view
														//H_AisViewCube->Set();
		H_AisViewCube->SetTransparency(0.8);
		
		hAISContext->Display(H_AisViewCube, Standard_True);

		H_AisViewCube->SetTransformPersistence(
			new Graphic3d_TransformPers(
				Graphic3d_TMF_TriedronPers,
				Aspect_TOTP_RIGHT_UPPER,
				Graphic3d_Vec2i(85, 85)));
	}
	return hAISContext;
}

Handle(AIS_InteractiveContext) OCCT_GraphDriver::CreateXYZAISContext(const Handle(V3d_Viewer)& hViewer)
{
	Handle(AIS_InteractiveContext) hAISContext = new AIS_InteractiveContext(hViewer); //��������ʽ����
	hAISContext->SetDisplayMode(AIS_Shaded, true);   //������ʾģʽΪ�ڱ�
	Handle(AIS_Point) Point = new AIS_Point(new Geom_CartesianPoint(0, 0, 0));
	hAISContext->Display(Point, Standard_False);
	Handle(AIS_Trihedron) Trihedron = new AIS_Trihedron(new Geom_Axis2Placement(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1))));
	Trihedron->SetSize(40);
	hAISContext->Display(Trihedron, Standard_False);
	hAISContext->SetZLayer(Trihedron, Graphic3d_ZLayerId_Topmost);
	hAISContext->SetTransformPersistence(Trihedron, Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers);//��ֹ����
	return hAISContext;
}

Handle(V3d_View) OCCT_GraphDriver::CreateView(const Handle(V3d_Viewer) & hViewer)
{
	Handle(V3d_View) hView;
	hView = hViewer->CreateView();
	Standard_Boolean myHlrModeIsOn = Standard_False;
	hView->SetComputedMode(myHlrModeIsOn);
	return hView;
}

