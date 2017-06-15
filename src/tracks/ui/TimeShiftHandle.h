/**********************************************************************

Audacity: A Digital Audio Editor

TimeShiftHandle.h

Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_TIMESHIFT_HANDLE__
#define __AUDACITY_TIMESHIFT_HANDLE__

#include "../../UIHandle.h"

#include "../../MemoryX.h"

#include "../../Snap.h"
#include "../../Track.h"

struct HitTestResult;
class WaveClip;

struct ClipMoveState {
   // non-NULL only if click was in a WaveTrack and without Shift key:
   WaveClip *capturedClip {};

   bool capturedClipIsSelection {};
   TrackArray trackExclusions {};
   double hSlideAmount {};
   TrackClipArray capturedClipArray {};
   wxInt64 snapLeft { -1 }, snapRight { -1 };

   void clear()
   {
      capturedClip = nullptr;
      capturedClipIsSelection = false;
      trackExclusions.clear();
      hSlideAmount = 0;
      capturedClipArray.clear();
      snapLeft = snapRight = -1;
   }
};

class TimeShiftHandle final : public UIHandle
{
   TimeShiftHandle();
   TimeShiftHandle(const TimeShiftHandle&) = delete;
   TimeShiftHandle &operator=(const TimeShiftHandle&) = delete;
   static TimeShiftHandle& Instance();
   static HitTestPreview HitPreview
      (const AudacityProject *pProject, bool unsafe);

public:
   // A utility function also used by menu commands
   static void CreateListOfCapturedClips
      ( ClipMoveState &state, const ViewInfo &viewInfo, Track &capturedTrack,
        TrackList &trackList, bool syncLocked, double clickTime );

   // A utility function also used by menu commands
   static void DoSlideHorizontal
      ( ClipMoveState &state, TrackList &trackList, Track &capturedTrack );

   static HitTestResult HitAnywhere(const AudacityProject *pProject);
   static HitTestResult HitTest
      (const wxMouseEvent &event, const wxRect &rect, const AudacityProject *pProject);

   virtual ~TimeShiftHandle();

   Result Click
      (const TrackPanelMouseEvent &event, AudacityProject *pProject) override;

   Result Drag
      (const TrackPanelMouseEvent &event, AudacityProject *pProject) override;

   HitTestPreview Preview
      (const TrackPanelMouseEvent &event, const AudacityProject *pProject)
      override;

   Result Release
      (const TrackPanelMouseEvent &event, AudacityProject *pProject,
       wxWindow *pParent) override;

   Result Cancel(AudacityProject *pProject) override;

   void DrawExtras
      (DrawingPass pass,
       wxDC * dc, const wxRegion &, const wxRect &panelRect) override;

   bool StopsOnKeystroke() override { return true; }

private:
   Track *mCapturedTrack{};
   wxRect mRect{};

   bool mDidSlideVertically{};
   bool mSlideUpDownOnly{};

   bool mSnapPreferRightEdge{};

   int mMouseClickX{};

   // Handles snapping the selection boundaries or track boundaries to
   // line up with existing tracks or labels.  mSnapLeft and mSnapRight
   // are the horizontal index of pixels to display user feedback
   // guidelines so the user knows when such snapping is taking place.
   std::unique_ptr<SnapManager> mSnapManager{};

   ClipMoveState mClipMoveState{};
};

#endif