#pragma once
#include <Includes/Includes.hpp>

namespace Custom
{
	namespace WaterMark
	{
		void Render( )
		{
			ImDrawList * DrawList = ImGui::GetBackgroundDrawList( );
			ImGui::TextColored(ImColor(80, 80, 80), std::to_string(ImGui::GetIO().Framerate).c_str());

			ImVec2 LogoSize = ImVec2( 60, 60 );

			const float Padding = 10;

			ImVec2 Size( 200, 40 ); 
			ImVec2 Pos( g_Variables.g_vGameWindowSize.x - Size.x, 10 );

			DrawList->AddRectFilled( Pos, Pos + Size, ImColor( g_Col.ChildCol ), 6, ImDrawFlags_RoundCornersLeft );

		}
	}

	namespace ArrayList
	{
		void Render( )
		{

		}
	}

}