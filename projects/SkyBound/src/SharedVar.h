#pragma once
class SharedVar
{
public:
	int GetRenderGroup();
	void SetRenderGroup(int rendergroup);

private:

	int RenderGroupBool = 0;
};

