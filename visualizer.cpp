#include "visualizer.h"

namespace OOFEM
{

long Visualizer::onCmdNewStructureViewer(FXObject*, FXSelector, void*)
{
    // Make new MDI child widget
    FXMDIChild* mdichild = new FXMDIChild(mdiclient, FXStringFormat("GL Viewer #%d", no), winappicon, mdimenu, MDI_TRACKING, 30, 30, 300, 200);

    // Create new visual context
    FXGLVisual *glvisual = new FXGLVisual(getApp(), VISUAL_DOUBLEBUFFER);
    m_glvisual.push_back(glvisual);

    // Create new view
    FXGLViewer *view=new FXGLViewer(mdichild, glvisual, this, ID_GLVIEWER);

    // Make a scene!
    FXGLGroup* scene = new FXGLGroup;
    m_scene.push_back(scene);

    // draw nodes
    for(std::size_t i = 0; i < mpStruct->NumberOfNodes(); ++i)
        scene->append(new FXGLPoint(mpStruct->GetNode(i)->X0(), mpStruct->GetNode(i)->Y0(), mpStruct->GetNode(i)->Z0()));

    // draw elements
    for(std::size_t i = 0; i < mpStruct->NumberOfElements(); ++i)
    {
        Entity::ConstPointer pElem = mpStruct->GetElement(i);
        if(pElem->GetGeometryType() == LINE_3D_2)
        {
            scene->append(new FXGLLine(pElem->GetNodes()[0]->X0(), pElem->GetNodes()[0]->Y0(), pElem->GetNodes()[0]->Z0(),
                                       pElem->GetNodes()[1]->X0(), pElem->GetNodes()[1]->Y0(), pElem->GetNodes()[1]->Z0()));
        }
    }

    // Add scenery
    view->setScene(scene);

    // Create the new window
    mdichild->create();

    // Next number
    ++no;
    return 1;
}

long Visualizer::onCmdNewDeformedStructureViewer(FXObject*, FXSelector, void*)
{
    return 1;
}

}

