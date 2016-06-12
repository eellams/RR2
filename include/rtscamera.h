// From:
//  http://irrlicht.sourceforge.net/forum//viewtopic.php?t=33870
//  and http://www.irrlicht3d.org/wiki/index.php?n=Main.RTSCameraByCmdKewin
//  thanks to xirtamatrix and Cmd Kewin

#ifndef __RTSCAMERA__
#define __RTSCAMERA__

#include <irrlicht.h>

// EE
//  removed these using statements
//  as was getting unexpected results when including this header file
//  they've just been moved to the source file
//  but now the header file looks so much messier... :(
/*
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
*/

class RTSCamera : public irr::scene::ICameraSceneNode
{
   public:
      RTSCamera(irr::IrrlichtDevice* devicepointer, irr::scene::ISceneNode* parent,
        irr::scene::ISceneManager* smgr, irr::s32 id,
         irr::f32 rotateSpeed = -1000.0f, irr::f32 zoomSpeed = 1000.0f, irr::f32 translationSpeed = 1000.0f);

      virtual ~RTSCamera();

      //Events
      virtual void render();
      virtual bool OnEvent(const irr::SEvent& event);
      virtual void OnRegisterSceneNode();
      virtual void OnAnimate(irr::u32 timeMs);

      //Setup
      virtual void setInputReceiverEnabled(bool enabled);
      virtual bool isInputReceiverEnabled() const;

      //Gets
      virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
      virtual const irr::core::matrix4& getProjectionMatrix() const;
      virtual const irr::scene::SViewFrustum* getViewFrustum() const;
      virtual const irr::core::vector3df& getTarget() const;
      virtual const irr::core::matrix4& getViewMatrix() const;
      virtual const irr::core::vector3df& getUpVector() const;
      virtual irr::f32 getNearValue() const;
      virtual irr::f32 getFarValue() const;
      virtual irr::f32 getAspectRatio() const;
      virtual irr::f32 getFOV() const;

      //Sets
      virtual void setNearValue(irr::f32 zn);
      virtual void setFarValue(irr::f32 zf);
      virtual void setAspectRatio(irr::f32 aspect);
      virtual void setFOV(irr::f32 fovy);
      virtual void setUpVector(const irr::core::vector3df& pos);
      virtual void setProjectionMatrix(const irr::core::matrix4& projection);
      virtual void setPosition(const irr::core::vector3df& newpos);
      virtual void setTarget(const irr::core::vector3df& newpos);

     virtual void setRotation(const irr::core::vector3df &) {}
     virtual void setProjectionMatrix(const irr::core::matrix4 &,bool) {}
     virtual void setViewMatrixAffector(const irr::core::matrix4 &) {}
     virtual const irr::core::matrix4& getViewMatrixAffector() const {return irr::core::IdentityMatrix;};
     virtual void bindTargetAndRotation(bool) {}
     virtual bool getTargetAndRotationBinding() const {return 0;}

      virtual void setZoomSpeed(irr::f32 value);
      virtual void setTranslateSpeed(irr::f32 value);
      virtual void setRotationSpeed(irr::f32 value);

      // Modified by EE
      //  sets the target height
      //  TODO proper cameera working
      virtual void setTargetHeight(irr::f32 height) {
        Target.Y = height;
      }


      //Helper Functions
      //virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);
      //virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);
      void pointCameraAtNode(ISceneNode* selectednode);
      void setMinZoom(irr::f32 amount);
      void setMaxZoom(irr::f32 amount);

      //Type Return
      virtual irr::scene::ESCENE_NODE_TYPE getType() const { return irr::scene::ESNT_CAMERA; }

      //Public Attributes
      bool atMinDistance;
      bool atMaxDistance;
      ISceneNode* selectednode;
   protected:
      //Properties
      irr::core::vector3df Target;
      irr::core::vector3df UpVector;
      irr::core::matrix4 Projection;
      irr::core::matrix4 View;
      irr::scene::SViewFrustum ViewArea;
      irr::core::aabbox3d<irr::f32> BBox;
      bool InputReceiverEnabled;
      irr::core::dimension2d<irr::f32> screenDim;
      irr::f32 Fovy;      // Field of view, in radians.
      irr::f32 Aspect;   // Aspect ratio.
      irr::f32 ZNear;   // value of the near view-plane.
      irr::f32 ZFar;   // Z-value of the far view-plane.

      void recalculateProjectionMatrix();
      void recalculateViewArea();

   private:
      irr::IrrlichtDevice* device;
      irr::core::vector3df Pos;
      bool zooming, rotating, moving, translating;
      irr::f32 zoomSpeed;
      irr::f32 translateSpeed;
      irr::f32 rotateSpeed;
      irr::f32 rotateStartX, rotateStartY;
      irr::f32 zoomStartX, zoomStartY;
      irr::f32 translateStartX, translateStartY;
      irr::f32 currentZoom;
      irr::f32 rotX, rotY;
      irr::core::vector3df oldTarget;
      irr::core::vector2df MousePos;
      bool Keys[irr::KEY_KEY_CODES_COUNT];
      bool MouseKeys[3];
      irr::f32 targetMinDistance;
      irr::f32 targetMaxDistance;

      enum MOUSE_BUTTON
      {
         MOUSE_BUTTON_RIGHT,
         MOUSE_BUTTON_MIDDLE,
         MOUSE_BUTTON_LEFT
      };

      void allKeysUp();
      void allMouseButtonsUp();
      bool isKeyDown(irr::s32 key);
      bool isMouseButtonDown(irr::s32 key);
      void animate();
      void updateAnimationState();
};

#endif
