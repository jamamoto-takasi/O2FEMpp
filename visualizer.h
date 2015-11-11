#pragma once

#include "define.h"
#include "structure.h"
#include "viewer/gl_view_window.h"

namespace OOFEM
{

class Visualizer : public FoxGL::GLViewWindow
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(Visualizer);

    Visualizer(std::string Name, Structure::Pointer pStruct, FXApp* a)
    : mName(Name), mpStruct(pStruct), FoxGL::GLViewWindow(a)
    {}

    virtual ~Visualizer()
    {}

    virtual long onCmdNewStructureViewer(FXObject*, FXSelector, void*);
    virtual long onCmdNewDeformedStructureViewer(FXObject*, FXSelector, void*);

private:
    std::string mName;
    Structure::ConstPointer mpStruct;
};

} // end namespace OOFEM

