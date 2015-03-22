#ifndef FILTER_HOUGH
#define FILTER_HOUGH


#include <windows.h>
#include "avisynth.h"

class InvertNeg : public GenericVideoFilter {
public:
   InvertNeg(PClip _child, IScriptEnvironment* env);
   PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif
