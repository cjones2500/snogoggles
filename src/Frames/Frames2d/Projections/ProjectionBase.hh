////////////////////////////////////////////////////////////////////////
/// \class Viewer::Frames::ProjectionBase
///
/// \brief   Base frame for projections
///
/// \author  Phil Jones <p.g.jones@qmul.ac.uk>
///
/// REVISION HISTORY:\n
///     21/02/12 : P.Jones - First Revision, new file. \n
///
/// \detail  All projection frame classes should just override the 
///          project function, which converts a 3d point into a 2d point.
///          The project function is pure virtual and hence this is an
///          abstract class
///
////////////////////////////////////////////////////////////////////////

#ifndef __Viewer_Frames_ProjectionBase__
#define __Viewer_Frames_ProjectionBase__

#include <SFML/System/Vector2.hpp>

#include <Viewer/Frame.hh>
#include <Viewer/Vector3.hh>

namespace Viewer
{
  class ProjectionImage;

namespace Frames
{

class ProjectionBase : public Frame
{
public:
  ProjectionBase( RectPtr rect ) : Frame( rect ) { }
  ~ProjectionBase();

  void Initialise();
  void LoadConfiguration( ConfigurationTable& configTable ) { }

  void SaveConfiguration( ConfigurationTable& configTable ) { }

  virtual void EventLoop();
  
  virtual std::string GetName() { return ProjectionBase::Name(); }
  
  static std::string Name() { return std::string( "ProjectionBase" ); }

  virtual void Render2d( RWWrapper& renderApp, 
			 const RenderState& renderState );

  void Render3d( RWWrapper& renderApp, 
		 const RenderState& renderState ) { }

  //EFrameType GetFrameType() { return eUtil; }
protected:
  void DrawLine( Vector3 v1, 
		 Vector3 v2 );
  void DrawHits();
  void DrawGeodesic();
  void DrawAllPMTs();

  virtual sf::Vector2<double> Project( Vector3 pmtPos ) = 0;
  ProjectionImage* fImage;
};

} // ::Frames

} // ::Viewer

#endif
