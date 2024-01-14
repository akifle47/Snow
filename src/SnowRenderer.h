#include <d3dx9.h>
#include <algorithm>
#include <string>
#include <fstream>

#include "rage/T_CB_Generic.h"
#include "rage/Device.h"
#include "rage/RenderTarget.h"
#include "rage/Viewport.h"
#include "rage/TextureFactory.h"
#include "rage/Weather.h"
#include "rage/LightSource.h"

namespace SnowRenderer
{
	void BuildRenderList();

	void OnBeforeLighting();
	void OnAfterCopyLight(rage::CLightSource *light);

	struct VertexFormat
	{
		float Pos[3];
		float TexCoord[2];
	};
};