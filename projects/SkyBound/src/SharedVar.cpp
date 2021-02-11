#include "SharedVar.h"


int SharedVar::GetRenderGroup()
{
	return RenderGroupBool;
}

void SharedVar::SetRenderGroup(int rendergroup)
{
	RenderGroupBool = rendergroup;
}
