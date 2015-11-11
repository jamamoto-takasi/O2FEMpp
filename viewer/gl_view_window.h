#ifndef _GL_VIEW_WINDOW_H_
#define _GL_VIEW_WINDOW_H_

#include <vector>
#include "fx.h"
#include "fx3d.h"
#include "resources.h"

namespace FoxGL
{

/*******************************************************************************/

// Main Window
class GLViewWindow : public FXMainWindow
{

    FXDECLARE(GLViewWindow)

private:

protected:
    FXMenuBar         *menubar;                 // Menu bar
    FXToolBar         *toolbar1;                // Tool bar
    FXToolBar         *toolbar2;                // Tool bar
    FXDockBar         *dockbar;                 // Dock bar
    FXToolBarShell    *dragshell1;              // Shell for floating menubar
    FXToolBarShell    *dragshell2;              // Shell for floating toolbar
    FXToolBarShell    *dragshell3;              // Shell for floating toolbar
    FXToolBarShell    *dragshell4;              // Shell for floating toolbar
    FXDockSite        *topdock;
    FXDockSite        *bottomdock;
    FXDockSite        *leftdock;
    FXDockSite        *rightdock;
    FXMDIClient       *mdiclient;               // MDI Client area
    FXStatusBar       *statusbar;               // Status bar
    FXMenuPane        *mdimenu;                 // MDI Window Menu
    FXTabBook         *panels;                  // Control panel
    std::vector<FXGLVisual*>        m_glvisual;           // Visual for OpenGL
    std::vector<FXGLViewer*>        m_view;               // 
    std::vector<FXMDIChild*>        m_mdichild;           // 
    std::vector<FXGLGroup*>         m_scene;              // 
    FXMenuPane        *filemenu;
    FXMenuPane        *editmenu;
    FXMenuPane        *viewmenu;
    FXMenuPane        *rendermenu;
    FXMenuPane        *windowmenu;
    FXMenuPane        *helpmenu;
    FXIcon            *frontviewicon;
    FXIcon            *backviewicon;
    FXIcon            *topviewicon;
    FXIcon            *bottomviewicon;
    FXIcon            *leftviewicon;
    FXIcon            *rightviewicon;
    FXIcon            *foxiconicon;
    FXIcon            *colorpalicon;
    FXIcon            *filesaveasicon;
    FXIcon            *filesaveicon;
    FXIcon            *fileopenicon;
    FXIcon            *printicon;
    FXIcon            *zoomicon;
    FXIcon            *filenewicon;
    FXIcon            *cuticon;
    FXIcon            *copyicon;
    FXIcon            *pasteicon;
    FXIcon            *killobjecticon;
    FXIcon            *nolighticon;
    FXIcon            *lighticon;
    FXIcon            *smoothlighticon;
    FXIcon            *parallelicon;
    FXIcon            *perspectiveicon;
    FXIcon            *newfoldericon;
    FXIcon            *penguinicon;
    FXIcon            *winappicon;
    FXIcon            *cameraicon;
    FXGLCanvas        *glcanvas;
    int               no; //window counter
  
    GLViewWindow(){}

public:

    // We define additional ID's, starting from the last one used by the base class+1.
    // This way, we know the ID's are all unique for this particular target.
    enum {
        ID_ABOUT=FXMainWindow::ID_LAST,
        ID_OPEN,
        ID_QUERY_MODE,
        ID_GLVIEWER,
        ID_CANVAS,
        ID_NEWVIEWER_1,
        ID_NEWVIEWER_2
    };

    // Message handlers
    long onCmdOpen(FXObject*,FXSelector,void*);
    long onCmdAbout(FXObject*,FXSelector,void*);
    long onUpdMode(FXObject*,FXSelector,void*);
    long onQueryMenu(FXObject*,FXSelector,void*);

    // extend more message handlers here
    virtual long onCmdNewStructureViewer(FXObject*,FXSelector,void*);
    virtual long onCmdNewDeformedStructureViewer(FXObject*,FXSelector,void*);

    // GLViewWindow constructor
    GLViewWindow(FXApp* a);

    // Initialize
    virtual void create();

    // GLViewWindow destructor
    virtual ~GLViewWindow();
};

} // end namespace FoxGL

/*******************************************************************************/
#endif

