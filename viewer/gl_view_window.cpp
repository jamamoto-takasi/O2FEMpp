#include <stdio.h>
#include "fx.h"
#include "fx3d.h"
#include "gl_view_window.h"
#include "fx_gl_shapes.h"

namespace FoxGL
{


// Message Map GLViewWindow class
FXDEFMAP(GLViewWindow) GLViewWindowMap[]={

  //__Message_Type_____________ID________________________Message_Handler_____
  FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ABOUT,      GLViewWindow::onCmdAbout),
  FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_OPEN,       GLViewWindow::onCmdOpen),
  FXMAPFUNC(SEL_UPDATE,      GLViewWindow::ID_QUERY_MODE, GLViewWindow::onUpdMode),
  FXMAPFUNC(SEL_COMMAND,     FXWindow::ID_QUERY_MENU,	  GLViewWindow::onQueryMenu),
  FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_NEWVIEWER_1,  GLViewWindow::onCmdNewStructureViewer),
  FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_NEWVIEWER_2,  GLViewWindow::onCmdNewDeformedStructureViewer)
};



// Macro for the GLViewWindow class hierarchy implementation
FXIMPLEMENT(GLViewWindow,FXMainWindow,GLViewWindowMap,ARRAYNUMBER(GLViewWindowMap))


/*******************************************************************************/

// Construct a GLViewWindow
GLViewWindow::GLViewWindow(FXApp* a):FXMainWindow(a,"OOFEM Structural Viewer",NULL,NULL,DECOR_ALL,0,0,800,600){
  no=1;

  penguinicon=new FXGIFIcon(getApp(),penguin);

  setIcon(penguinicon);

  FXColorDialog *colordlg=new FXColorDialog(this,"Color Dialog",DECOR_TITLE|DECOR_BORDER);

  // Make status bar
  statusbar=new FXStatusBar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|STATUSBAR_WITH_DRAGCORNER);

  // Site where to dock
  topdock=new FXDockSite(this,DOCKSITE_NO_WRAP|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  bottomdock=new FXDockSite(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  leftdock=new FXDockSite(this,LAYOUT_SIDE_LEFT|LAYOUT_FILL_Y);
  rightdock=new FXDockSite(this,LAYOUT_SIDE_RIGHT|LAYOUT_FILL_Y);

  // Menubar 1
  dragshell1=new FXToolBarShell(this,FRAME_RAISED);
  menubar=new FXMenuBar(topdock,dragshell1,LAYOUT_DOCK_SAME|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(menubar,menubar,FXMenuBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Tool bar 2
  dragshell2=new FXToolBarShell(this,FRAME_RAISED);
  toolbar1=new FXToolBar(topdock,dragshell2,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|FRAME_RAISED);
  new FXToolBarGrip(toolbar1,toolbar1,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Tool bar 3
  dragshell3=new FXToolBarShell(this,FRAME_RAISED);
  toolbar2=new FXToolBar(topdock,dragshell3,LAYOUT_DOCK_SAME|LAYOUT_SIDE_TOP|FRAME_RAISED);
  new FXToolBarGrip(toolbar2,toolbar2,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // The good old penguin, what would we be without it?
  new FXButton(statusbar,"\tHello, I'm Tux...\nThe symbol for the Linux Operating System.\nAnd all it stands for.",penguinicon,NULL,0,LAYOUT_RIGHT);

  // Contents
  FXHorizontalFrame *frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0, 4,4);

  // Nice sunken box around GL viewer
  FXVerticalFrame *box=new FXVerticalFrame(frame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

  // MDI Client
  mdiclient=new FXMDIClient(box,LAYOUT_FILL_X|LAYOUT_FILL_Y);

  // Icon for MDI Child
  winappicon=new FXGIFIcon(getApp(),winapp);

  // Make MDI Window Menu
  mdimenu=new FXMDIMenu(this,mdiclient);

  // MDI buttons in menu:- note the message ID's!!!!!
  // Normally, MDI commands are simply sensitized or desensitized;
  // Under the menubar, however, they're hidden if the MDI Client is
  // not maximized.  To do this, they must have different ID's.
  new FXMDIWindowButton(menubar,mdimenu,mdiclient,FXMDIClient::ID_MDI_MENUWINDOW,LAYOUT_LEFT|LAYOUT_CENTER_Y);
  new FXMDIDeleteButton(menubar,mdiclient,FXMDIClient::ID_MDI_MENUCLOSE,FRAME_RAISED|LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  new FXMDIRestoreButton(menubar,mdiclient,FXMDIClient::ID_MDI_MENURESTORE,FRAME_RAISED|LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  new FXMDIMinimizeButton(menubar,mdiclient,FXMDIClient::ID_MDI_MENUMINIMIZE,FRAME_RAISED|LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  // Tool bar 4
  dragshell4=new FXToolBarShell(this,FRAME_RAISED|FRAME_THICK);
  dockbar=new FXDockBar(rightdock,dragshell4,LAYOUT_FILL_Y|LAYOUT_SIDE_RIGHT, 0,0,0,0, 2,2,2,2, 2,2);

  // Only dock left and right
  dockbar->allowedSides(FXDockBar::ALLOW_LEFT|FXDockBar::ALLOW_RIGHT);

  // Nice caption over dock
  FXHorizontalFrame *dockframe=new FXHorizontalFrame(dockbar,LAYOUT_SIDE_TOP|LAYOUT_FILL_X, 0,0,0,0, 0,0,0,0, 0,0);
  new FXDockTitle(dockframe,"Controls",dockbar,FXToolBar::ID_TOOLBARGRIP,LAYOUT_FILL_X|FRAME_SUNKEN|JUSTIFY_CENTER_X);
  new FXMDIDeleteButton(dockframe,dockbar,FXWindow::ID_HIDE,LAYOUT_FILL_Y);

  // Tab book with switchable panels
  panels=new FXTabBook(dockbar,NULL,0,LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

  new FXTabItem(panels,"Angles\tCamera Angles\tSwitch to camera angles panel.");

  // Angles
  FXMatrix *angles=new FXMatrix(panels,3,FRAME_THICK|FRAME_RAISED|MATRIX_BY_COLUMNS|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
  new FXLabel(angles,"X:");
  new FXTextField(angles,6,mdiclient,FXGLViewer::ID_ROLL,TEXTFIELD_INTEGER|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  FXDial* x_dial=new FXDial(angles,mdiclient,FXGLViewer::ID_DIAL_X,FRAME_SUNKEN|FRAME_THICK|DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_CENTER_Y,0,0,160,14,0,0,0,0);
  x_dial->setTipText("Rotate about X");
  x_dial->setNotchOffset(900);

  new FXLabel(angles,"Y:");
  new FXTextField(angles,6,mdiclient,FXGLViewer::ID_PITCH,TEXTFIELD_INTEGER|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  FXDial* y_dial=new FXDial(angles,mdiclient,FXGLViewer::ID_DIAL_Y,FRAME_SUNKEN|FRAME_THICK|DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_CENTER_Y,0,0,160,14,0,0,0,0);
  y_dial->setTipText("Rotate about Y");
  y_dial->setNotchOffset(900);

  new FXLabel(angles,"Z:");
  new FXTextField(angles,6,mdiclient,FXGLViewer::ID_YAW,TEXTFIELD_INTEGER|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  FXDial* z_dial=new FXDial(angles,mdiclient,FXGLViewer::ID_DIAL_Z,FRAME_SUNKEN|FRAME_THICK|DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_CENTER_Y,0,0,160,14,0,0,0,0);
  z_dial->setTipText("Rotate about Z");
  z_dial->setNotchOffset(900);

  new FXLabel(angles,"FOV:");
  FXTextField* fov=new FXTextField(angles,5,mdiclient,FXGLViewer::ID_FOV,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  new FXFrame(angles,0);
  fov->setTipText("Field of view");

  new FXLabel(angles,"Zoom:");
  FXTextField* zz=new FXTextField(angles,5,mdiclient,FXGLViewer::ID_ZOOM,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  new FXFrame(angles,0);
  zz->setTipText("Zooming");

  new FXLabel(angles,"Scale X:");
  new FXTextField(angles,5,mdiclient,FXGLViewer::ID_SCALE_X,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  new FXFrame(angles,0);
  new FXLabel(angles,"Scale Y:");
  new FXTextField(angles,5,mdiclient,FXGLViewer::ID_SCALE_Y,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  new FXFrame(angles,0);
  new FXLabel(angles,"Scale Z:");
  new FXTextField(angles,5,mdiclient,FXGLViewer::ID_SCALE_Z,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK);
  new FXFrame(angles,0);

  // Colors
  new FXTabItem(panels,"Colors\tColors\tSwitch to color panel.");
  FXMatrix *colors=new FXMatrix(panels,2,MATRIX_BY_COLUMNS|FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_Y|LAYOUT_CENTER_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
  new FXLabel(colors,"Background:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_BACK_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);

  new FXLabel(colors,"Top Back:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_TOP_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);
  new FXLabel(colors,"Bottom Back:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_BOTTOM_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);


  new FXLabel(colors,"Ambient:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_AMBIENT_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);


  new FXLabel(colors,"Light Amb:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_LIGHT_AMBIENT,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);
  new FXLabel(colors,"Light Diff:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_LIGHT_DIFFUSE,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);
  new FXLabel(colors,"Light Spec:",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y|JUSTIFY_RIGHT);
  new FXColorWell(colors,0,mdiclient,FXGLViewer::ID_LIGHT_SPECULAR,COLORWELL_OPAQUEONLY|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0 ,0,0,0,0);

  // Switches
  new FXTabItem(panels,"Settings\tSettings\tSwitch to settings panel.");
  FXVerticalFrame *settings=new FXVerticalFrame(panels,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_Y|LAYOUT_CENTER_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
  new FXCheckButton(settings,"Lighting",mdiclient,FXGLViewer::ID_LIGHTING,ICON_BEFORE_TEXT);
  new FXCheckButton(settings,"Fog",mdiclient,FXGLViewer::ID_FOG,ICON_BEFORE_TEXT);
  new FXCheckButton(settings,"Dither",mdiclient,FXGLViewer::ID_DITHER,ICON_BEFORE_TEXT);
  new FXCheckButton(settings,"Turbo",mdiclient,FXGLViewer::ID_TURBO,ICON_BEFORE_TEXT);


  filenewicon=new FXGIFIcon(getApp(),filenew);
  fileopenicon=new FXGIFIcon(getApp(),fileopen);
  filesaveicon=new FXGIFIcon(getApp(),filesave);
  filesaveasicon=new FXGIFIcon(getApp(),filesaveas,0,IMAGE_ALPHAGUESS);
  newfoldericon=new FXGIFIcon(getApp(),newfolder);

  // File Menu
  filemenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&File",NULL,filemenu);
  new FXMenuCommand(filemenu,"&New...\tCtl-N\tCreate new document.",filenewicon);
  new FXMenuCommand(filemenu,"&Open...\tCtl-O\tOpen document file.",fileopenicon,this,ID_OPEN);
  new FXMenuCommand(filemenu,"&Save\tCtl-S\tSave document.",filesaveicon);
  new FXMenuCommand(filemenu,"Save &As...\t\tSave document to another file.",filesaveasicon);
  new FXMenuCommand(filemenu,"&Print Image...\t\tPrint snapshot image.",NULL,mdiclient,FXGLViewer::ID_PRINT_IMAGE,MENU_AUTOGRAY);
  new FXMenuCommand(filemenu,"&Print Vector...\t\tPrint geometry.",NULL,mdiclient,FXGLViewer::ID_PRINT_VECTOR,MENU_AUTOGRAY);
  new FXMenuCommand(filemenu,"&Dump...\t\tDump widgets.",NULL,getApp(),FXApp::ID_DUMP);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q\tQuit the application.",NULL,getApp(),FXApp::ID_QUIT);

  // Edit Menu
  editmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Edit",NULL,editmenu);
  new FXMenuCommand(editmenu,"Lasso",NULL,mdiclient,FXGLViewer::ID_LASSO_SELECT,MENU_AUTOGRAY);
  new FXMenuCommand(editmenu,"Copy\tCtl-C",NULL,mdiclient,FXGLViewer::ID_COPY_SEL,MENU_AUTOGRAY);
  new FXMenuCommand(editmenu,"Cut\tCtl-X",NULL,mdiclient,FXGLViewer::ID_CUT_SEL,MENU_AUTOGRAY);
  new FXMenuCommand(editmenu,"Paste\tCtl-V",NULL,mdiclient,FXGLViewer::ID_PASTE_SEL,MENU_AUTOGRAY);
  new FXMenuCommand(editmenu,"Delete",NULL,mdiclient,FXGLViewer::ID_DELETE_SEL,MENU_AUTOGRAY);


  // File manipulation
  new FXButton(toolbar1,"\tNew\tCreate new document.",filenewicon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tOpen\tOpen document file.",fileopenicon,this,ID_OPEN,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tSave\tSave document.",filesaveicon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tSave As\tSave document to another file.",filesaveasicon,this,ID_OPEN,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tNew Folder\tNo comment",newfoldericon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  printicon=new FXGIFIcon(getApp(),print);

  // Print
  new FXSeparator(toolbar1,SEPARATOR_GROOVE);
  new FXButton(toolbar1,"\tPrint Image\tPrint shapshot image.",printicon,mdiclient,FXGLViewer::ID_PRINT_IMAGE,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  cuticon=new FXGIFIcon(getApp(),cut);
  copyicon=new FXGIFIcon(getApp(),copy);
  pasteicon=new FXGIFIcon(getApp(),paste);

  // Editing
  new FXSeparator(toolbar1,SEPARATOR_GROOVE);
  new FXButton(toolbar1,"\tCut",cuticon,mdiclient,FXGLViewer::ID_CUT_SEL,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tCopy",copyicon,mdiclient,FXGLViewer::ID_COPY_SEL,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar1,"\tPaste",pasteicon,mdiclient,FXGLViewer::ID_PASTE_SEL,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  perspectiveicon=new FXGIFIcon(getApp(),perspective);
  parallelicon=new FXGIFIcon(getApp(),parallel);

  // Projections
  new FXButton(toolbar2,"\tPerspective\tSwitch to perspective projection.",perspectiveicon,mdiclient,FXGLViewer::ID_PERSPECTIVE,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tParallel\tSwitch to parallel projection.",parallelicon,mdiclient,FXGLViewer::ID_PARALLEL,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  nolighticon=new FXGIFIcon(getApp(),nolight);
  lighticon=new FXGIFIcon(getApp(),light);
  smoothlighticon=new FXGIFIcon(getApp(),smoothlight);

  // Shading model
  new FXSeparator(toolbar2,SEPARATOR_GROOVE);
  new FXButton(toolbar2,"\tNo shading\tTurn light sources off.",nolighticon,mdiclient,FXGLShape::ID_SHADEOFF,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tFlat shading\tTurn on faceted (flat) shading.",lighticon,mdiclient,FXGLShape::ID_SHADEON,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tSmooth shading\tTurn on smooth shading.",smoothlighticon,mdiclient,FXGLShape::ID_SHADESMOOTH,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  new FXSeparator(toolbar2,SEPARATOR_GROOVE);
  new FXToggleButton(toolbar2,"\tToggle Light\tToggle light source.",FXString::null,nolighticon,lighticon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  frontviewicon=new FXGIFIcon(getApp(),frontview);
  backviewicon=new FXGIFIcon(getApp(),backview);
  topviewicon=new FXGIFIcon(getApp(),topview,0,IMAGE_OPAQUE);
  bottomviewicon=new FXGIFIcon(getApp(),bottomview,0,IMAGE_OPAQUE);
  leftviewicon=new FXGIFIcon(getApp(),leftview);
  rightviewicon=new FXGIFIcon(getApp(),rightview);

  // View orientation
  new FXSeparator(toolbar2,SEPARATOR_GROOVE);
  new FXButton(toolbar2,"\tFront View\tView objects from the front.",frontviewicon,mdiclient,FXGLViewer::ID_FRONT,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tBack View\tView objects from behind.",backviewicon,mdiclient,FXGLViewer::ID_BACK,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tLeft View\tView objects from the left.",leftviewicon,mdiclient,FXGLViewer::ID_LEFT,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tRight View\tView objects from the right.",rightviewicon,mdiclient,FXGLViewer::ID_RIGHT,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tTop View\tView objects from the top.",topviewicon,mdiclient,FXGLViewer::ID_TOP,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tBottom View\tView objects from below.",bottomviewicon,mdiclient,FXGLViewer::ID_BOTTOM,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);


  colorpalicon=new FXGIFIcon(getApp(),colorpal);
  zoomicon=new FXGIFIcon(getApp(),zoom);
  cameraicon=new FXGIFIcon(getApp(),camera,0,IMAGE_OPAQUE);
  foxiconicon=new FXGIFIcon(getApp(),foxicon);

  // Miscellaneous buttons
  new FXSeparator(toolbar2,SEPARATOR_GROOVE);
  new FXButton(toolbar2,FXString::null,zoomicon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,"\tColors\tDisplay color dialog.",colorpalicon,colordlg,FXWindow::ID_SHOW,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,FXString::null,cameraicon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar2,FXString::null,foxiconicon,NULL,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  killobjecticon=new FXGIFIcon(getApp(),killobject);

  // Dangerous delete a bit on the side
  new FXSeparator(toolbar1,SEPARATOR_GROOVE);
  new FXButton(toolbar1,"\tDelete\tDelete the selected object.",killobjecticon,mdiclient,FXGLViewer::ID_DELETE_SEL,BUTTON_AUTOGRAY|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // View menu
  viewmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&View",NULL,viewmenu);
  new FXMenuRadio(viewmenu,"Parallel\t\tSwitch to parallel projection.",mdiclient,FXGLViewer::ID_PARALLEL,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"Perspective\t\tSwitch to perspective projection.",mdiclient,FXGLViewer::ID_PERSPECTIVE,MENU_AUTOGRAY);
  new FXMenuSeparator(viewmenu);
  new FXMenuRadio(viewmenu,"&Front\tCtl-F\tFront view.",mdiclient,FXGLViewer::ID_FRONT,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"&Back\tCtl-B\tBack view.",mdiclient,FXGLViewer::ID_BACK,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"&Left\tCtl-L\tLeft view.",mdiclient,FXGLViewer::ID_LEFT,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"&Right\tCtl-R\tRight view.",mdiclient,FXGLViewer::ID_RIGHT,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"&Top\tCtl-T\tTop view.",mdiclient,FXGLViewer::ID_TOP,MENU_AUTOGRAY);
  new FXMenuRadio(viewmenu,"&Bottom\tCtl-K\tBottom view.",mdiclient,FXGLViewer::ID_BOTTOM,MENU_AUTOGRAY);
  new FXMenuSeparator(viewmenu);
  new FXMenuCommand(viewmenu,"F&it\t\tFit to view.",NULL,mdiclient,FXGLViewer::ID_FITVIEW,MENU_AUTOGRAY);
  new FXMenuCommand(viewmenu,"R&eset\tCtl-G\tReset all viewing parameters",NULL,mdiclient,FXGLViewer::ID_RESETVIEW,MENU_AUTOGRAY);
  new FXMenuCommand(viewmenu,"Zoom\t\tZoom in on area",NULL,mdiclient,FXGLViewer::ID_LASSO_ZOOM,MENU_AUTOGRAY);
  new FXMenuCommand(viewmenu,"Select\t\tZoom in on area",NULL,mdiclient,FXGLViewer::ID_LASSO_SELECT,MENU_AUTOGRAY);

  // Rendering menu
  rendermenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Rendering",NULL,rendermenu);
  new FXMenuCheck(rendermenu,"Points\t\tRender as points.",mdiclient,FXGLShape::ID_STYLE_POINTS,MENU_AUTOGRAY);
  new FXMenuCheck(rendermenu,"Wire Frame\t\tRender as wire frame.",mdiclient,FXGLShape::ID_STYLE_WIREFRAME,MENU_AUTOGRAY);
  new FXMenuCheck(rendermenu,"Surface \t\tRender solid surface.",mdiclient,FXGLShape::ID_STYLE_SURFACE,MENU_AUTOGRAY);
  new FXMenuCheck(rendermenu,"Bounding Box\t\tRender bounding box only.",mdiclient,FXGLShape::ID_STYLE_BOUNDINGBOX,MENU_AUTOGRAY);

  // Window menu
  windowmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Windows",NULL,windowmenu);
  new FXMenuCommand(windowmenu,"New Structure Viewer\t\tCreate new structure viewer window.",NULL,this,ID_NEWVIEWER_1);
  new FXMenuCommand(windowmenu,"New Deformed Structure Viewer\t\tCreate new deformed structure viewer window.",NULL,this,ID_NEWVIEWER_2);
  new FXMenuCommand(windowmenu,"Tile Horizontally\t\tTile windows horizontally.",NULL,mdiclient,FXMDIClient::ID_MDI_TILEHORIZONTAL);
  new FXMenuCommand(windowmenu,"Tile Vertically\t\tTile windows vertically.",NULL,mdiclient,FXMDIClient::ID_MDI_TILEVERTICAL);
  new FXMenuCommand(windowmenu,"Cascade\t\tCascade windows.",NULL,mdiclient,FXMDIClient::ID_MDI_CASCADE);
  new FXMenuCommand(windowmenu,"Delete\t\tDelete current viewer window.",NULL,mdiclient,FXMDIClient::ID_MDI_CLOSE);
  new FXMenuSeparator(windowmenu);
  new FXMenuCheck(windowmenu,"Main Toolbar",toolbar1,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(windowmenu,"Graphic Toolbar",toolbar2,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(windowmenu,"Control panel",dockbar,FXWindow::ID_TOGGLESHOWN);
  FXMenuSeparator* sep1=new FXMenuSeparator(windowmenu);
  sep1->setTarget(mdiclient);
  sep1->setSelector(FXMDIClient::ID_MDI_ANY);
  new FXMenuRadio(windowmenu,FXString::null,mdiclient,FXMDIClient::ID_MDI_1);
  new FXMenuRadio(windowmenu,FXString::null,mdiclient,FXMDIClient::ID_MDI_2);
  new FXMenuRadio(windowmenu,FXString::null,mdiclient,FXMDIClient::ID_MDI_3);
  new FXMenuRadio(windowmenu,FXString::null,mdiclient,FXMDIClient::ID_MDI_4);

  // Help menu
  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Help",NULL,helpmenu,LAYOUT_RIGHT);
  new FXMenuCommand(helpmenu,"&About FOX...\t\tDisplay FOX about panel.",NULL,this,ID_ABOUT,0);

  // Make a tool tip
  new FXToolTip(getApp(),0);

  // The status bar shows our mode
  statusbar->getStatusLine()->setTarget(this);
  statusbar->getStatusLine()->setSelector(ID_QUERY_MODE);
}


// Destructor
GLViewWindow::~GLViewWindow(){
  for(int i = 0; i < m_scene.size(); ++i) delete m_scene[i];
  for(int i = 0; i < m_glvisual.size(); ++i) delete m_glvisual[i];
  delete menubar;                 // Menu bar
  delete toolbar1;                // Tool bar
  delete toolbar2;                // Tool bar
  delete dockbar;                 // Dock bar
  delete dragshell1;              // Shell for floating menubar
  delete dragshell2;              // Shell for floating toolbar
  delete dragshell3;              // Shell for floating toolbar
  delete dragshell4;              // Shell for floating toolbar
  delete topdock;
  delete bottomdock;
  delete leftdock;
  delete rightdock;
  delete mdiclient;               // MDI Client area
  delete statusbar;               // Status bar
  delete mdimenu;                 // MDI Window Menu
  delete filemenu;
  delete editmenu;
  delete viewmenu;
  delete rendermenu;
  delete windowmenu;
  delete helpmenu;
  delete frontviewicon;
  delete backviewicon;
  delete topviewicon;
  delete bottomviewicon;
  delete leftviewicon;
  delete rightviewicon;
  delete foxiconicon;
  delete colorpalicon;
  delete filesaveasicon;
  delete filesaveicon;
  delete fileopenicon;
  delete printicon;
  delete zoomicon;
  delete filenewicon;
  delete cuticon;
  delete copyicon;
  delete pasteicon;
  delete killobjecticon;
  delete nolighticon;
  delete lighticon;
  delete smoothlighticon;
  delete parallelicon;
  delete perspectiveicon;
  delete newfoldericon;
  delete penguinicon;
  delete winappicon;
  delete cameraicon;
//  delete panels;
}



// Create and initialize
void GLViewWindow::create(){
  FXMainWindow::create();
  dragshell1->create();
  dragshell2->create();
  dragshell3->create();
  //toolbar1->undock();
  show(PLACEMENT_SCREEN);
}


// About
long GLViewWindow::onCmdAbout(FXObject*,FXSelector,void*){
  FXMessageBox::information(this,MBOX_OK,"About FOX","FOX OpenGL Example.\nCopyright (C) 1998,2005 Jeroen van der Zijp");
  return 1;
}


// Open
long GLViewWindow::onCmdOpen(FXObject*,FXSelector,void*){
  const FXchar patterns[]="All Files (*)\nC++ Source Files (*.cpp,*.cxx,*.c++,*.C)\nC++ Header Files (*.hpp,*.hxx,*.h++,*.H,*.h)\nObject Files (*.o)\nHTML Header Files (*.html,*.htm)";
  FXFileDialog open(this,"Open some file");
  open.setPatternList(patterns);
  if(open.execute()){
    fxmessage("File=%s\n",open.getFilename().text());
    }
  return 1;
}



// Make new viewer
long GLViewWindow::onCmdNewStructureViewer(FXObject*,FXSelector,void*){
  // Make new MDI child widget
  FXMDIChild* mdichild=new FXMDIChild(mdiclient,FXStringFormat("GL Viewer #%d",no),winappicon,mdimenu,MDI_TRACKING,30,30,300,200);

  // Create new visual context
  FXGLVisual *glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
  m_glvisual.push_back(glvisual);
  
  // Create new view
  FXGLViewer *view=new FXGLViewer(mdichild,glvisual,this,ID_GLVIEWER);

  // Make a scene!
  FXGLGroup* scene=new FXGLGroup;
  m_scene.push_back(scene);
//   scene->append(new FXGLPoint(2,0,0));
//   scene->append(new FXGLPoint(0,2,0));
//   scene->append(new FXGLPoint(2,2,0));
//   scene->append(new FXGLPoint(0,0,0));
//   scene->append(new FXGLLine(0,0,0, 1,0,0));
//   scene->append(new FXGLLine(0,0,0, 0,1,0));
//   scene->append(new FXGLLine(0,0,0, 0,0,1));
//   scene->append(new FXGLLine(0,0,0, 1,1,1));
  FXGLGroup *gp2=new FXGLGroup;
  scene->append(gp2);
  FXGLSphere *sphere=new FXGLSphere(1.0, 1.0, 0.0, 0.5);
  FXGLSphere *sphere2=new FXGLSphere(0.0, 0.0, 0.0, 0.8);
  sphere->setTipText("Sphere");
  gp2->append(new FXGLCube(-1.0, 0.0, 0.0,  1.0, 1.0, 1.0));
  gp2->append(new FXGLCube( 1.0, 0.0, 0.0,  1.0, 1.0, 1.0));
  gp2->append(new FXGLCube( 0.0,-1.0, 0.0,  1.0, 1.0, 1.0));
  gp2->append(new FXGLCube( 0.0, 1.0, 0.0,  1.0, 1.0, 1.0));
  gp2->append(new FXGLCone(1.0,-1.5, 0.0, 1.0, 0.5));
  gp2->append(new FXGLCylinder(-1.0, 0.5, 0.0, 1.0, 0.5));
  gp2->append(sphere);
  gp2->append(sphere2);

  // Add scenery
  view->setScene(scene);

  // Create the new window
  mdichild->create();

  // Next number
  no++;
  return 1;
}

// Make new animation
long GLViewWindow::onCmdNewDeformedStructureViewer(FXObject*,FXSelector,void*){
  // Make new MDI child widget
  FXMDIChild* mdichild=new FXMDIChild(mdiclient,FXStringFormat("GL Viewer #%d",no),winappicon,mdimenu,MDI_TRACKING,30,30,300,200);
  
  // Create visual context
  FXGLVisual *glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER|VISUAL_STEREO);
  m_glvisual.push_back(glvisual);

  // Create new view
  FXGLViewer *view=new FXGLViewer(mdichild,glvisual,this,ID_GLVIEWER);
  
  // Make a scene!
  FXGLGroup* scene=new FXGLGroup;
  m_scene.push_back(scene);
  FXGLGroup *gp2=new FXGLGroup;
  scene->append(gp2);
  FXGLDemoShape* shape = new FXGLDemoShape();
  gp2->append(shape);
  shape->createTab(panels,"Animation\tAnimation control\tetc");
  
  // Add scenery
  view->setScene(scene);
  
  // Create the new window
  mdichild->create();
  
  // Next number
  no++;
  return 1;
}


// Statusbar wanted to know about mode
// We fake it here for the purpose of demonstration...
long GLViewWindow::onUpdMode(FXObject* sender,FXSelector,void*){
  FXStatusLine *statusline=(FXStatusLine*)sender;
  statusline->setText("Ready.");
  return 1;
}


/*
*  When the user right-clicks in the GLViewer background, the viewer first sends
*  a SEL_COMMAND message with identifier FXWindow::ID_QUERY_MENU to the selected
*  GLObject (if any). If that message isn't handled, it tries to send it to the
*  GLViewer's target (which in our case is the main window).
*/
long GLViewWindow::onQueryMenu(FXObject* sender,FXSelector,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXMenuPane pane(this);
  new FXMenuRadio(&pane,"Parallel\t\tSwitch to parallel projection.",sender,FXGLViewer::ID_PARALLEL);
  new FXMenuRadio(&pane,"Perspective\t\tSwitch to perspective projection.",sender,FXGLViewer::ID_PERSPECTIVE);
  new FXMenuSeparator(&pane);
  new FXMenuRadio(&pane,"&Front\t\tFront view.",sender,FXGLViewer::ID_FRONT);
  new FXMenuRadio(&pane,"&Back\t\tBack view.",sender,FXGLViewer::ID_BACK);
  new FXMenuRadio(&pane,"&Left\t\tLeft view.",sender,FXGLViewer::ID_LEFT);
  new FXMenuRadio(&pane,"&Right\t\tRight view.",sender,FXGLViewer::ID_RIGHT);
  new FXMenuRadio(&pane,"&Top\t\tTop view.",sender,FXGLViewer::ID_TOP);
  new FXMenuRadio(&pane,"&Bottom\t\tBottom view.",sender,FXGLViewer::ID_BOTTOM);
  new FXMenuSeparator(&pane);
  new FXMenuCommand(&pane,"F&it\t\tFit to view.",NULL,sender,FXGLViewer::ID_FITVIEW);
  new FXMenuCommand(&pane,"R&eset\t\tReset all viewing parameters",NULL,sender,FXGLViewer::ID_RESETVIEW);
  pane.create();
  pane.popup(NULL,event->root_x,event->root_y);
  getApp()->runModalWhileShown(&pane);
  return 1;
}

}

