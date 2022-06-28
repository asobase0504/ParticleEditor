//**************************************************
//
// Hackathon ( imgui_property.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "imgui_property.h"
#include "main.h"
#include "file.h"
#include "letter.h"
#include "utility.h"
#include <imgui_internal.h>
#include <assert.h>
#include <implot.h>
#include <imgui_widget_flamegraph.h>


//------------------------------
//CPU
//------------------------------
#include <stdio.h>
#include <Windows.h>

//------------------------
//GPU
//------------------------
#include <nvml.h>

#if _DEBUG
#pragma comment(lib, "nvml.lib")
#else
#pragma comment(lib, "nvml.lib")
#endif


//==================================================
// マクロ定義
//==================================================
#define IMGUI_DEFINE_MATH_OPERATORS


//==================================================
// 定義
//==================================================
static const int	MAX_TEXT = 1024;		// テキストの最大文字数
static const char*	WINDOW_NAME = "test";	// ウインドウの名前 (キャプションに表示)

//==================================================
// スタティック変数
//==================================================
static char FileString[MAX_PATH * 256];	// ファイル名
static bool	s_window = false;	// ウインドウを使用するかどうか
static FileParticleData imguiParticle;	// ImGuiに保存されてるパーティクル情報
static bool s_bEffectEnable = false;
static float s_fScale = 50.0f;
static const unsigned int gpu_id = 0;
static nvmlDevice_t device;
static ImVec2 foo[10];

namespace nl = nlohmann;

nl::json Sin;//リストの生成
 // init data so editor knows to take it from here
//unsigned MemoryUsageMegaBytes(void)
//{
//	MEMORYSTATUSEX m = { sizeof m };
//	GlobalMemoryStatusEx(&m);
//	return (unsigned)(((512 * 1024) + (m.ullTotalVirtual - m.ullAvailVirtual)) / (1024 * 1024));
//}

// [src] https://github.com/ocornut/imgui/issues/123
// [src] https://github.com/ocornut/imgui/issues/55

// v1.22 - flip button; cosmetic fixes
// v1.21 - oops :)
// v1.20 - add iq's interpolation code
// v1.10 - easing and colors        
// v1.00 - jari komppa's original

#pragma once

#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <cmath>

/* To use, add this prototype somewhere..

namespace ImGui
{
int Curve(const char *label, const ImVec2& size, int maxpoints, ImVec2 *points);
float CurveValue(float p, int maxpoints, const ImVec2 *points);
float CurveValueSmooth(float p, int maxpoints, const ImVec2 *points);
};

*/
/*
Example of use:

ImVec2 foo[10];
...
foo[0].x = -1; // init data so editor knows to take it from here
...
if (ImGui::Curve("Das editor", ImVec2(600, 200), 10, foo))
{
// curve changed
}
...
float value_you_care_about = ImGui::CurveValue(0.7f, 10, foo); // calculate value at position 0.7
*/

namespace tween {
	enum TYPE
	{
		LINEAR,

		QUADIN,          // t^2
		QUADOUT,
		QUADINOUT,
		CUBICIN,         // t^3
		CUBICOUT,
		CUBICINOUT,
		QUARTIN,         // t^4
		QUARTOUT,
		QUARTINOUT,
		QUINTIN,         // t^5
		QUINTOUT,
		QUINTINOUT,
		SINEIN,          // sin(t)
		SINEOUT,
		SINEINOUT,
		EXPOIN,          // 2^t
		EXPOOUT,
		EXPOINOUT,
		CIRCIN,          // sqrt(1-t^2)
		CIRCOUT,
		CIRCINOUT,
		ELASTICIN,       // exponentially decaying sine wave
		ELASTICOUT,
		ELASTICINOUT,
		BACKIN,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
		BACKOUT,
		BACKINOUT,
		BOUNCEIN,        // exponentially decaying parabolic bounce
		BOUNCEOUT,
		BOUNCEINOUT,

		SINESQUARE,      // gapjumper's
		EXPONENTIAL,     // gapjumper's
		SCHUBRING1,      // terry schubring's formula 1
		SCHUBRING2,      // terry schubring's formula 2
		SCHUBRING3,      // terry schubring's formula 3

		SINPI2,          // tomas cepeda's
		SWING,           // tomas cepeda's & lquery's
	};

	// }

	// implementation

	static inline
		double ease(int easetype, double t)
	{
		using namespace std;

		const double d = 1.f;
		const double pi = 3.1415926535897932384626433832795;
		const double pi2 = 3.1415926535897932384626433832795 / 2;

		double p = t / d;

		switch (easetype)
		{
			// Modeled after the line y = x
		default:
		case TYPE::LINEAR: {
			return p;
		}

						   // Modeled after the parabola y = x^2
		case TYPE::QUADIN: {
			return p * p;
		}

						   // Modeled after the parabola y = -x^2 + 2x
		case TYPE::QUADOUT: {
			return -(p * (p - 2));
		}

							// Modeled after the piecewise quadratic
							// y = (1/2)((2x)^2)             ; [0, 0.5)
							// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
		case TYPE::QUADINOUT: {
			if (p < 0.5) {
				return 2 * p * p;
			}
			else {
				return (-2 * p * p) + (4 * p) - 1;
			}
		}

							  // Modeled after the cubic y = x^3
		case TYPE::CUBICIN: {
			return p * p * p;
		}

							// Modeled after the cubic y = (x - 1)^3 + 1
		case TYPE::CUBICOUT: {
			double f = (p - 1);
			return f * f * f + 1;
		}

							 // Modeled after the piecewise cubic
							 // y = (1/2)((2x)^3)       ; [0, 0.5)
							 // y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
		case TYPE::CUBICINOUT: {
			if (p < 0.5) {
				return 4 * p * p * p;
			}
			else {
				double f = ((2 * p) - 2);
				return 0.5 * f * f * f + 1;
			}
		}

							   // Modeled after the quartic x^4
		case TYPE::QUARTIN: {
			return p * p * p * p;
		}

							// Modeled after the quartic y = 1 - (x - 1)^4
		case TYPE::QUARTOUT: {
			double f = (p - 1);
			return f * f * f * (1 - p) + 1;
		}

							 // Modeled after the piecewise quartic
							 // y = (1/2)((2x)^4)        ; [0, 0.5)
							 // y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
		case TYPE::QUARTINOUT: {
			if (p < 0.5) {
				return 8 * p * p * p * p;
			}
			else {
				double f = (p - 1);
				return -8 * f * f * f * f + 1;
			}
		}

							   // Modeled after the quintic y = x^5
		case TYPE::QUINTIN: {
			return p * p * p * p * p;
		}

							// Modeled after the quintic y = (x - 1)^5 + 1
		case TYPE::QUINTOUT: {
			double f = (p - 1);
			return f * f * f * f * f + 1;
		}

							 // Modeled after the piecewise quintic
							 // y = (1/2)((2x)^5)       ; [0, 0.5)
							 // y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
		case TYPE::QUINTINOUT: {
			if (p < 0.5) {
				return 16 * p * p * p * p * p;
			}
			else {
				double f = ((2 * p) - 2);
				return  0.5 * f * f * f * f * f + 1;
			}
		}

							   // Modeled after quarter-cycle of sine wave
		case TYPE::SINEIN: {
			return sin((p - 1) * pi2) + 1;
		}

						   // Modeled after quarter-cycle of sine wave (different phase)
		case TYPE::SINEOUT: {
			return sin(p * pi2);
		}

							// Modeled after half sine wave
		case TYPE::SINEINOUT: {
			return 0.5 * (1 - cos(p * pi));
		}

							  // Modeled after shifted quadrant IV of unit circle
		case TYPE::CIRCIN: {
			return 1 - sqrt(1 - (p * p));
		}

						   // Modeled after shifted quadrant II of unit circle
		case TYPE::CIRCOUT: {
			return sqrt((2 - p) * p);
		}

							// Modeled after the piecewise circular function
							// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
							// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
		case TYPE::CIRCINOUT: {
			if (p < 0.5) {
				return 0.5 * (1 - sqrt(1 - 4 * (p * p)));
			}
			else {
				return 0.5 * (sqrt(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
			}
		}

							  // Modeled after the exponential function y = 2^(10(x - 1))
		case TYPE::EXPOIN: {
			return (p == 0.0) ? p : pow(2, 10 * (p - 1));
		}

						   // Modeled after the exponential function y = -2^(-10x) + 1
		case TYPE::EXPOOUT: {
			return (p == 1.0) ? p : 1 - pow(2, -10 * p);
		}

							// Modeled after the piecewise exponential
							// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
							// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
		case TYPE::EXPOINOUT: {
			if (p == 0.0 || p == 1.0) return p;

			if (p < 0.5) {
				return 0.5 * pow(2, (20 * p) - 10);
			}
			else {
				return -0.5 * pow(2, (-20 * p) + 10) + 1;
			}
		}

							  // Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
		case TYPE::ELASTICIN: {
			return sin(13 * pi2 * p) * pow(2, 10 * (p - 1));
		}

							  // Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
		case TYPE::ELASTICOUT: {
			return sin(-13 * pi2 * (p + 1)) * pow(2, -10 * p) + 1;
		}

							   // Modeled after the piecewise exponentially-damped sine wave:
							   // y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
							   // y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
		case TYPE::ELASTICINOUT: {
			if (p < 0.5) {
				return 0.5 * sin(13 * pi2 * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
			}
			else {
				return 0.5 * (sin(-13 * pi2 * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);
			}
		}

								 // Modeled (originally) after the overshooting cubic y = x^3-x*sin(x*pi)
		case TYPE::BACKIN: { /*
							 return p * p * p - p * sin(p * pi); */
			double s = 1.70158f;
			return p * p * ((s + 1) * p - s);
		}

						   // Modeled (originally) after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
		case TYPE::BACKOUT: { /*
							  double f = (1 - p);
							  return 1 - (f * f * f - f * sin(f * pi)); */
			double s = 1.70158f;
			return --p, 1.f * (p*p*((s + 1)*p + s) + 1);
		}

							// Modeled (originally) after the piecewise overshooting cubic function:
							// y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
							// y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
		case TYPE::BACKINOUT: { /*
								if(p < 0.5) {
								double f = 2 * p;
								return 0.5 * (f * f * f - f * sin(f * pi));
								}
								else {
								double f = (1 - (2*p - 1));
								return 0.5 * (1 - (f * f * f - f * sin(f * pi))) + 0.5;
								} */
			double s = 1.70158f * 1.525f;
			if (p < 0.5) {
				return p *= 2, 0.5 * p * p * (p*s + p - s);
			}
			else {
				return p = p * 2 - 2, 0.5 * (2 + p*p*(p*s + p + s));
			}
		}

#           define tween$bounceout(p) ( \
                (p) < 4/11.0 ? (121 * (p) * (p))/16.0 : \
                (p) < 8/11.0 ? (363/40.0 * (p) * (p)) - (99/10.0 * (p)) + 17/5.0 : \
                (p) < 9/10.0 ? (4356/361.0 * (p) * (p)) - (35442/1805.0 * (p)) + 16061/1805.0 \
                           : (54/5.0 * (p) * (p)) - (513/25.0 * (p)) + 268/25.0 )

		case TYPE::BOUNCEIN: {
			return 1 - tween$bounceout(1 - p);
		}

		case TYPE::BOUNCEOUT: {
			return tween$bounceout(p);
		}

		case TYPE::BOUNCEINOUT: {
			if (p < 0.5) {
				return 0.5 * (1 - tween$bounceout(1 - p * 2));
			}
			else {
				return 0.5 * tween$bounceout((p * 2 - 1)) + 0.5;
			}
		}

#           undef tween$bounceout

		case TYPE::SINESQUARE: {
			double A = sin((p)*pi2);
			return A*A;
		}

		case TYPE::EXPONENTIAL: {
			return 1 / (1 + exp(6 - 12 * (p)));
		}

		case TYPE::SCHUBRING1: {
			return 2 * (p + (0.5f - p)*abs(0.5f - p)) - 0.5f;
		}

		case TYPE::SCHUBRING2: {
			double p1pass = 2 * (p + (0.5f - p)*abs(0.5f - p)) - 0.5f;
			double p2pass = 2 * (p1pass + (0.5f - p1pass)*abs(0.5f - p1pass)) - 0.5f;
			double pAvg = (p1pass + p2pass) / 2;
			return pAvg;
		}

		case TYPE::SCHUBRING3: {
			double p1pass = 2 * (p + (0.5f - p)*abs(0.5f - p)) - 0.5f;
			double p2pass = 2 * (p1pass + (0.5f - p1pass)*abs(0.5f - p1pass)) - 0.5f;
			return p2pass;
		}

		case TYPE::SWING: {
			return ((-cos(pi * p) * 0.5) + 0.5);
		}

		case TYPE::SINPI2: {
			return sin(p * pi2);
		}
		}
	}
}

namespace ImGui
{
	// [src] http://iquilezles.org/www/articles/minispline/minispline.htm
	// key format (for dim == 1) is (t0,x0,t1,x1 ...)
	// key format (for dim == 2) is (t0,x0,y0,t1,x1,y1 ...)
	// key format (for dim == 3) is (t0,x0,y0,z0,t1,x1,y1,z1 ...)
	void spline(const float *key, int num, int dim, float t, float *v)
	{
		static signed char coefs[16] = {
			-1, 2,-1, 0,
			3,-5, 0, 2,
			-3, 4, 1, 0,
			1,-1, 0, 0 };

		const int size = dim + 1;

		// find key
		int k = 0; while (key[k*size] < t) k++;

		// interpolant
		const float h = (t - key[(k - 1)*size]) / (key[k*size] - key[(k - 1)*size]);

		// init result
		for (int i = 0; i < dim; i++) v[i] = 0.0f;

		// add basis functions
		for (int i = 0; i<4; i++)
		{
			int kn = k + i - 2; if (kn<0) kn = 0; else if (kn>(num - 1)) kn = num - 1;

			const signed char *co = coefs + 4 * i;

			const float b = 0.5f*(((co[0] * h + co[1])*h + co[2])*h + co[3]);

			for (int j = 0; j < dim; j++) v[j] += b * key[kn*size + j + 1];
		}
	}

	float CurveValueSmooth(float p, int maxpoints, const ImVec2 *points)
	{
		if (maxpoints < 2 || points == 0)
			return 0;
		if (p < 0) return points[0].y;

		float *input = new float[maxpoints * 2];
		float output[4];

		for (int i = 0; i < maxpoints; ++i) {
			input[i * 2 + 0] = points[i].x;
			input[i * 2 + 1] = points[i].y;
		}

		spline(input, maxpoints, 1, p, output);

		delete[] input;
		return output[0];
	}

	float CurveValue(float p, int maxpoints, const ImVec2 *points)
	{
		if (maxpoints < 2 || points == 0)
			return 0;
		if (p < 0) return points[0].y;

		int left = 0;
		while (left < maxpoints && points[left].x < p && points[left].x != -1) left++;
		if (left) left--;

		if (left == maxpoints - 1)
			return points[maxpoints - 1].y;

		float d = (p - points[left].x) / (points[left + 1].x - points[left].x);

		return points[left].y + (points[left + 1].y - points[left].y) * d;
	}

	int Curve(const char *label, const ImVec2& size, const int maxpoints, ImVec2 *points)
	{
		int modified = 0;
		int i;
		if (maxpoints < 2 || points == 0)
			return 0;

		if (points[0].x < 0)
		{
			points[0].x = 0;
			points[0].y = 0;
			points[1].x = 1;
			points[1].y = 1;
			points[2].x = -1;
		}

		ImGuiWindow* window = GetCurrentWindow();
		//ImGuiState& g = *GImGui;
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		if (window->SkipItems)
			return 0;

		ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
		ItemSize(bb);
		if (!ItemAdd(bb, NULL))
			return 0;

		// const bool hovered = IsHovered(bb, id);
		const bool hovered = ItemHoverable(bb, id);
		int max = 0;
		while (max < maxpoints && points[max].x >= 0) max++;

		int kill = 0;
		do
		{
			if (kill)
			{
				modified = 1;
				for (i = kill + 1; i < max; i++)
				{
					points[i - 1] = points[i];
				}
				max--;
				points[max].x = -1;
				kill = 0;
			}

			for (i = 1; i < max - 1; i++)
			{
				if (abs(points[i].x - points[i - 1].x) < 1 / 128.0)
				{
					kill = i;
				}
			}
		} while (kill);


		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);

		float ht = bb.Max.y - bb.Min.y;
		float wd = bb.Max.x - bb.Min.x;

		if (hovered)
		{
			SetHoveredID(id);
			if (g.IO.MouseDown[0])
			{
				modified = 1;
				ImVec2 pos = (g.IO.MousePos - bb.Min) / (bb.Max - bb.Min);
				pos.y = 1 - pos.y;

				int left = 0;
				while (left < max && points[left].x < pos.x) left++;
				if (left) left--;

				ImVec2 p = points[left] - pos;
				float p1d = sqrt(p.x*p.x + p.y*p.y);
				p = points[left + 1] - pos;
				float p2d = sqrt(p.x*p.x + p.y*p.y);
				int sel = -1;
				if (p1d < (1 / 16.0)) sel = left;
				if (p2d < (1 / 16.0)) sel = left + 1;

				if (sel != -1)
				{
					points[sel] = pos;
				}
				else
				{
					if (max < maxpoints)
					{
						max++;
						for (i = max; i > left; i--)
						{
							points[i] = points[i - 1];
						}
						points[left + 1] = pos;
					}
					if (max < maxpoints)
						points[max].x = -1;
				}

				// snap first/last to min/max
				if (points[0].x < points[max - 1].x) {
					points[0].x = 0;
					points[max - 1].x = 1;
				}
				else {
					points[0].x = 1;
					points[max - 1].x = 0;
				}
			}
		}

		// bg grid
		window->DrawList->AddLine(
			ImVec2(bb.Min.x, bb.Min.y + ht / 2),
			ImVec2(bb.Max.x, bb.Min.y + ht / 2),
			GetColorU32(ImGuiCol_TextDisabled), 3);

		window->DrawList->AddLine(
			ImVec2(bb.Min.x, bb.Min.y + ht / 4),
			ImVec2(bb.Max.x, bb.Min.y + ht / 4),
			GetColorU32(ImGuiCol_TextDisabled));

		window->DrawList->AddLine(
			ImVec2(bb.Min.x, bb.Min.y + ht / 4 * 3),
			ImVec2(bb.Max.x, bb.Min.y + ht / 4 * 3),
			GetColorU32(ImGuiCol_TextDisabled));

		for (i = 0; i < 9; i++)
		{
			window->DrawList->AddLine(
				ImVec2(bb.Min.x + (wd / 10) * (i + 1), bb.Min.y),
				ImVec2(bb.Min.x + (wd / 10) * (i + 1), bb.Max.y),
				GetColorU32(ImGuiCol_TextDisabled));
		}

		// smooth curve
		enum { smoothness = 256 }; // the higher the smoother
		for (i = 0; i <= (smoothness - 1); ++i) {
			float px = (i + 0) / float(smoothness);
			float qx = (i + 1) / float(smoothness);
			float py = 1 - CurveValueSmooth(px, maxpoints, points);
			float qy = 1 - CurveValueSmooth(qx, maxpoints, points);
			ImVec2 p(px * (bb.Max.x - bb.Min.x) + bb.Min.x, py * (bb.Max.y - bb.Min.y) + bb.Min.y);
			ImVec2 q(qx * (bb.Max.x - bb.Min.x) + bb.Min.x, qy * (bb.Max.y - bb.Min.y) + bb.Min.y);
			window->DrawList->AddLine(p, q, GetColorU32(ImGuiCol_PlotLines));
		}

		// lines
		for (i = 1; i < max; i++)
		{
			//直線
			ImVec2 a = points[i - 1];
			ImVec2 b = points[i];
			a.y = 1 - a.y;
			b.y = 1 - b.y;
			a = a * (bb.Max - bb.Min) + bb.Min;
			b = b * (bb.Max - bb.Min) + bb.Min;
			window->DrawList->AddLine(a, b, GetColorU32(ImGuiCol_PlotLinesHovered));
		}

		if (hovered)
		{
			//曲線
			// control points
			for (i = 0; i < max; i++)
			{
				ImVec2 p = points[i];
				p.y = 1 - p.y;
				p = p * (bb.Max - bb.Min) + bb.Min;
				ImVec2 a = p - ImVec2(2, 2);
				ImVec2 b = p + ImVec2(2, 2);
				window->DrawList->AddRect(a, b, GetColorU32(ImGuiCol_PlotLinesHovered));
			}
		}

		// buttons; @todo: mirror, smooth, tessellate
		if (ImGui::Button("Flip")) {
			for (i = 0; i < max; ++i) {
				points[i].y = 1 - points[i].y;
			}
		}
		ImGui::SameLine();

		// curve selector
		const char* items[] = {
			"Custom",

			"Linear",
			"Quad in",
			"Quad out",
			"Quad in  out",
			"Cubic in",
			"Cubic out",
			"Cubic in  out",
			"Quart in",
			"Quart out",
			"Quart in  out",
			"Quint in",
			"Quint out",
			"Quint in  out",
			"Sine in",
			"Sine out",
			"Sine in  out",
			"Expo in",
			"Expo out",
			"Expo in  out",
			"Circ in",
			"Circ out",
			"Circ in  out",
			"Elastic in",
			"Elastic out",
			"Elastic in  out",
			"Back in",
			"Back out",
			"Back in  out",
			"Bounce in",
			"Bounce out",
			"Bounce in out",

			"Sine square",
			"Exponential",

			"Schubring1",
			"Schubring2",
			"Schubring3",

			"SinPi2",
			"Swing"
		};
		static int item = 0;
		if (modified) {
			item = 0;
		}
		if (ImGui::Combo("Ease type", &item, items, IM_ARRAYSIZE(items))) {
			max = maxpoints;
			if (item > 0) {
				for (i = 0; i < max; ++i) {
					points[i].x = i / float(max - 1);
					points[i].y = float(tween::ease(item - 1, points[i].x));
				}
			}
		}

		char buf[128];
		const char *str = label;

		if (hovered) {
			ImVec2 pos = (g.IO.MousePos - bb.Min) / (bb.Max - bb.Min);
			pos.y = 1 - pos.y;

			sprintf(buf, "%s (%f,%f)", label, pos.x, pos.y);
			str = buf;
		}

		RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL, ImVec2(0.5f, 0.5f));

		return modified;
	}

};


//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitImguiProperty(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
#ifdef _DEBUG

	s_window = true;

	// ウインドウの表示
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// 文字の設定
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// スタイルの設定
	ImGui::StyleColorsDark();

	// タイトルバーの色設定
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// スライドの色設定
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// チェックマークの色設定
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// スクロールの色設定
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));

	// プラットフォームの設定/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);


	//GPU
	nvmlInit();//初期化
	nvmlDeviceGetHandleByIndex(gpu_id, &device);
	foo[0].x = -1;
#endif // _DEBUG
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitImguiProperty(HWND hWnd, WNDCLASSEX wcex)
{
#ifdef _DEBUG

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	// ウインドウの破壊
	::DestroyWindow(hWnd);

	// ウインドウクラスの登録を解除
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

#endif // _DEBUG
}


////--------------------------------------------------
//// 曲線制作サインカーブ
////--------------------------------------------------
//namespace ImGui
//{
//	template<int steps>
//	void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) 
//	{
//		static float C[(steps + 1) * 4], *K = 0;
//		if (!K) 
//		{
//			K = C;
//			for (unsigned step = 0; step <= steps; ++step) 
//			{
//				float t = (float)step / (float)steps;
//				C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
//				C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
//				C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
//				C[step * 4 + 3] = t*t*t;               // * P3
//			}
//		}
//		for (unsigned step = 0; step <= steps; ++step) 
//		{
//			ImVec2 point = {
//				K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
//				K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
//			};
//			results[step] = point;
//		}
//	}
//
//	float BezierValue(float dt01, float P[4]) 
//	{
//		enum { STEPS = 512 };
//		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
//		ImVec2 results[STEPS + 1];
//		bezier_table<STEPS>(Q, results);
//		return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
//	}
//
//	int Bezier(const char *label, float P[4]) {
//		// visuals
//		enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
//		enum { CURVE_WIDTH = 4 }; // main curved line width
//		enum { LINE_WIDTH = 1 }; // handlers: small lines width
//		enum { GRAB_RADIUS = 6 }; // handlers: circle radius
//		enum { GRAB_BORDER = 2 }; // handlers: circle border width
//
//		const ImGuiStyle& Style = GetStyle();
//		const ImGuiIO& IO = GetIO();
//		ImDrawList* DrawList = GetWindowDrawList();
//		ImGuiWindow* Window = GetCurrentWindow();
//		if (Window->SkipItems)
//			return false;
//
//		// header and spacing
//		int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
//		int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
//		Dummy(ImVec2(0, 3));
//
//		// prepare canvas
//		const float avail = GetContentRegionAvail().x;
//		const float dim = ImMin(avail, 128.f);
//
//		/*width, height*/
//		ImVec2 Canvas(dim, dim);
//
//		ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
//		ItemSize(bb);
//		if (!ItemAdd(bb, NULL))
//			return changed;
//
//		const ImGuiID id = Window->GetID(label);
//		// hovered |= 0 != IsItemHovered(ImRect(bb.Min, bb.Min + ImVec2(avail,dim)), id);
//
//		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);
//
//		// background grid
//		for (int i = 0; i <= Canvas.x; i += (int)(Canvas.x * 0.25f)) {
//			DrawList->AddLine(
//				ImVec2(bb.Min.x + i, bb.Min.y),
//				ImVec2(bb.Min.x + i, bb.Max.y),
//				GetColorU32(ImGuiCol_TextDisabled));
//		}
//		for (int i = 0; i <= Canvas.y; i += (int)(Canvas.y * 0.25f)) {
//			DrawList->AddLine(
//				ImVec2(bb.Min.x, bb.Min.y + i),
//				ImVec2(bb.Max.x, bb.Min.y + i),
//				GetColorU32(ImGuiCol_TextDisabled));
//		}
//
//		// eval curve
//		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
//		ImVec2 results[SMOOTHNESS + 1];
//		bezier_table<SMOOTHNESS>(Q, results);
//
//		// control points: 2 lines and 2 circles
//		{
//			char buf[128];
//			sprintf(buf, "0##%s", label);
//
//			// handle grabbers
//			for (int i = 0; i < 2; ++i)
//			{
//				ImVec2 pos = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
//				SetCursorScreenPos(pos - ImVec2(GRAB_RADIUS, GRAB_RADIUS));
//				InvisibleButton((buf[0]++, buf), ImVec2(2 * GRAB_RADIUS, 2 * GRAB_RADIUS));
//				if (IsItemActive() || IsItemHovered())
//				{
//					SetTooltip("(%4.3f, %4.3f)", P[i * 2 + 0], P[i * 2 + 1]);
//				}
//				if (IsItemActive() && IsMouseDragging(0))
//				{
//					P[i * 2 + 0] += GetIO().MouseDelta.x / Canvas.x;
//					P[i * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y;
//					changed = true;
//				}
//			}
//
//			if (hovered || changed) DrawList->PushClipRectFullScreen();
//
//			// draw curve
//			{
//				ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
//				for (int i = 0; i < SMOOTHNESS; ++i) {
//					ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
//					ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
//					ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
//					ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
//					DrawList->AddLine(r, s, color, CURVE_WIDTH);
//				}
//			}
//
//			// draw lines and grabbers
//			float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
//			ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
//			ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
//			ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
//			ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
//			DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
//			DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
//			DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
//			DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
//			DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
//			DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));
//
//			if (hovered || changed) DrawList->PopClipRect();
//
//			// restore cursor pos
//			SetCursorScreenPos(ImVec2(bb.Min.x, bb.Max.y + GRAB_RADIUS)); // :P
//		}
//
//		return changed;
//	}
//}
//void ShowDemo_DragLines()
//{
//	ImGui::BulletText("水平線と垂直線をクリックしてドラッグします。 ");
//	static double x1 = 0.2;
//	static double x2 = 0.8;
//	static double y1 = 0.25;
//	static double y2 = 0.75;
//	static double f = 0.1;
//	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
//	ImGui::SetNextWindowPos(ImVec2());
//	ImGui::Begin("AAA",nullptr,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
//	ImVec2 graphWindowSize = ImGui::GetContentRegionAvail(); // ImGui :: GetWindowSize();
//	constexpr size_t GraphAmount = 15;
//	ImVec2 oneGraphSize = ImVec2(-1,graphWindowSize.y / GraphAmount);
//	if (oneGraphSize.y < 150.f)
//	{
//		oneGraphSize.y = 150.f;
//	}
//}
//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateImguiProperty(void)
{
#ifdef _DEBUG

	if (!s_window)
	{// ウインドウを使用しない
		return;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	static int mode = 0;
	static int sliderInt = 0;
	static float sliderFloat = 0;
	static bool checkBox = true;
	static char text[MAX_TEXT] = "";

	static bool useEffect = false;
	static bool s_bRot = false;
	static bool s_bTexRot = false;
	static bool s_bUsesrand = false;

	// ウインドウの起動時の場所
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(650, 400), ImGuiCond_Once);

	// ウインドウの命名
	ImGui::Begin(WINDOW_NAME, nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				sliderInt = 2;
			}
			else if (ImGui::MenuItem("Load"))
			{
				sliderInt = 5;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	//パーティクルのデータ出力＆読み込み
	if (ImGui::Button("DATA"))
	{
		imguiParticle = GetStatus();
	}
	if (ImGui::Button("OUT"))
	{
		OutputStatus();
	}

	//SINカーブのデータ出力＆読み込み
	if (ImGui::Button("SINDATA"))
	{
		const std::string pathToJSON = "data/FILE/Sintest.json";
		std::ifstream ifs(pathToJSON);

		if (ifs)
		{
			//StringToWString(UTF8toSjis(j["name"]));
			//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
			ifs >> Sin;
			
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				std::string name = "Sin";
				std::string Number = std::to_string(nCnt);
				name += Number;

				foo[nCnt] = ImVec2(Sin[name]["X"], Sin[name]["Y"]);	
			}
		}
	}
	if (ImGui::Button("SINOUT"))
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			
			std::string name = "Sin";
			std::string Number = std::to_string(nCnt);
			name += Number;
			Sin[name] = { { "X", foo[nCnt].x } ,{ "Y", foo[nCnt].y } };

		}
		auto jobj = Sin.dump();
		std::ofstream writing_file;
		const std::string pathToJSON = "data/FILE/Sintest.json";
		writing_file.open(pathToJSON, std::ios::out);
		writing_file << jobj << std::endl;
		writing_file.close();
	}
	
	if (ImGui::Curve("Das editor", ImVec2(600, 200), 10, foo))
	{
		float value_you_care_about = ImGui::CurveValue(0.7f, 10, foo); // calculate value at position 0.7
	}

	//ImGui::Text("CPU1  : %d", MemoryUsageMegaBytes());
	//void* const p = malloc(512 * 1024 * 1024);
	// テキスト表示
	//ImGui::Text("CPU2  : %d", MemoryUsageMegaBytes());
	//free(p);
	//ImGui::Text("CPU3  : %d", MemoryUsageMegaBytes());

	unsigned int clockMZ = 0;

	//現在の使用率取得
	nvmlDeviceGetClock(device, NVML_CLOCK_MEM, NVML_CLOCK_ID_CURRENT, &clockMZ);

	float clockNau = (float)clockMZ *0.1f;

	// テキスト表示
	ImGui::Text("GPU  : %.2f%%", clockNau);

	//グラフ
	static float v[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	
	//グラフの四角からでないようにするやつ
	{
		if (v[0] <= 0.0f)
		{
			v[0] = 0.0f;
		}

		if (v[0] >= 1.0f)
		{
			v[0] = 1.0f;
		}

		if (v[1] <= 0.0f)
		{
			v[1] = 0.0f;
		}

		if (v[1] >= 1.0f)
		{
			v[1] = 1.0f;
		}

		if (v[2] >= 1.0f)
		{
			v[2] = 1.0f;
		}

		if (v[2] <= 0.0f)
		{
			v[2] = 0.0f;
		}

		if (v[3] >= 1.0f)
		{
			v[3] = 1.0f;
		}

		if (v[3] <= 0.0f)
		{
			v[3] = 0.0f;
		}
	}

	// テキスト表示
	ImGui::Text("FPS  : %.2f", ImGui::GetIO().Framerate);
	ImGui::Separator();


	//エフェクト関係
	if (ImGui::TreeNode("Effecttree1", "EffectSetting"))
	{
		imguiParticle.color.a = 1.0f;
		imguiParticle.particle.color.destCol.a = 1.0f;

		//imguiParticle.fScale = 50.0f;

		//ここTEXよみこみ
		if (ImGui::Button("LOAD TEXTURE"))
		{
			GetFile(nullptr, FileString, sizeof(FileString), TEXT("C:\\"));
		}

		if (ImGui::Checkbox("EffectEnable", &useEffect))
		{
			if (!s_bEffectEnable)
			{
				s_bEffectEnable = true;
			}
			else if (s_bEffectEnable)
			{
				s_bEffectEnable = false;
				imguiParticle.particle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				imguiParticle.particle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				imguiParticle.particle.nLife = 60;
				imguiParticle.particle.fRadius = 0.5f;
			}
		}

		if (ImGui::Button("default"))
		{
			imguiParticle.pos.x = SCREEN_WIDTH * 0.5f;
			imguiParticle.pos.y = SCREEN_HEIGHT * 0.5f;
			imguiParticle.particle.maxPopPos.x = 0.0f;
			imguiParticle.particle.maxPopPos.y = 0.0f;
			imguiParticle.particle.minPopPos.x = 0.0f;
			imguiParticle.particle.minPopPos.y = 0.0f;
			imguiParticle.particle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.particle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
			imguiParticle.particle.color.destCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			imguiParticle.particle.color.colRandamMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			imguiParticle.particle.color.colRandamMin = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			imguiParticle.particle.color.colTransition = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			imguiParticle.particle.nLife = 60;
			imguiParticle.particle.fScale = 50.0f;
			imguiParticle.particle.fRadius = 4.5f;
			imguiParticle.particle.fAngle = 20.5f;
			imguiParticle.particle.fAttenuation = 0.98f;
			imguiParticle.particle.alphaBlend = (CParticle::ALPHABLENDTYPE)0;
		}

		//EffectData *Effect = GetStatus();
		ImGui::SliderFloat("PosX", &imguiParticle.pos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("PosY", &imguiParticle.pos.y, 0, (float)SCREEN_HEIGHT);

		// 生成範囲の設定
		ImGui::SliderFloat("MaxPopPosX", &imguiParticle.particle.maxPopPos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("MinPopPosX", &imguiParticle.particle.minPopPos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("MaxPopPosY", &imguiParticle.particle.maxPopPos.y, 0, (float)SCREEN_HEIGHT);
		ImGui::SliderFloat("MinPopPosY", &imguiParticle.particle.minPopPos.y, 0, (float)SCREEN_HEIGHT);

		ImGui::InputFloat3("SettingEffectMove", imguiParticle.particle.move, "%f");
		ImGui::SliderFloat("MoveX", &imguiParticle.particle.move.x, -100.0f, 100.0f);
		ImGui::SliderFloat("MoveY", &imguiParticle.particle.move.y, -100.0f, 100.0f);

		//詳細
		if (ImGui::TreeNode("Effecttree2", "Details"))
		{
			//rot計算用
			static float s_fDeg = 0.0f;

			ImGui::InputFloat3("SettingEffectRot", imguiParticle.particle.rot, "%f");
			ImGui::SliderFloat("Rot", &s_fDeg, -D3DX_PI, D3DX_PI);

			if (ImGui::Checkbox("BackRot", &s_bRot))
			{
				imguiParticle.particle.bBackrot = !imguiParticle.particle.bBackrot;
			}

			float rotX = imguiParticle.pos.x * cosf(s_fDeg) + imguiParticle.pos.x * sinf(s_fDeg);
			float rotY = imguiParticle.pos.y * sinf(s_fDeg) - imguiParticle.pos.y * cosf(s_fDeg);
			float fAngle = atan2f(rotX, rotY);
			imguiParticle.particle.rot = D3DXVECTOR3(rotX, rotY, fAngle);
			
			if (imguiParticle.particle.rot.z > D3DX_PI)
			{
				imguiParticle.particle.rot.z -= D3DX_PI * 2;
			}
			else if (imguiParticle.particle.rot.z < -D3DX_PI)
			{
				imguiParticle.particle.rot.z += D3DX_PI * 2;
			}

			ImGui::SliderFloat("TextureScale", &imguiParticle.particle.fScale, 0.0f, 100.0f);
			ImGui::SliderInt("Life", &imguiParticle.particle.nLife, 0, 500);
			ImGui::SliderFloat("Radius", &imguiParticle.particle.fRadius, 0.0f, 100.0f);
			ImGui::SliderAngle("Angle", &imguiParticle.particle.fAngle, 0.0f, 2000.0f);
			ImGui::SliderFloat("Attenuation", &imguiParticle.particle.fAttenuation, 0.0f, 1.0f);

			//挙動おかしくなっちゃった時用
			if (ImGui::Button("DataRemove"))
			{
				//DeleteParticleAll();
				//RemoveAngle();
			}

			//ツリーを閉じる
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Effecttree3", "Color"))
		{
			//カラーパレット
			ImGui::ColorEdit4("clear color", (float*)&imguiParticle.particle.color);

			// ランダムカラー
			ImGui::Checkbox("ColorRandom", &imguiParticle.particle.color.bColRandom);

			if (imguiParticle.particle.color.bColRandom)
			{
				ImGui::ColorEdit4("clear RandamMax", (float*)&imguiParticle.particle.color.colRandamMax);
				ImGui::ColorEdit4("clear RandamMin", (float*)&imguiParticle.particle.color.colRandamMin);
			}

			// カラートラディション
			ImGui::Checkbox("ColorTransition", &imguiParticle.particle.color.bColTransition);

			if (imguiParticle.particle.color.bColTransition)
			{// 目的の色
				ImGui::ColorEdit4("clear destColor", (float*)&imguiParticle.particle.color.destCol);

				ImGui::Checkbox("RandomTransitionTime", &imguiParticle.particle.color.bRandomTransitionTime);

				if (!imguiParticle.particle.color.bRandomTransitionTime)
				{
					ImGui::SliderInt("EndTime", &imguiParticle.particle.color.nEndTime, 0, imguiParticle.particle.nLife);
				}
			}

			//ツリーを閉じる
			ImGui::TreePop();
		}

		//グラデーション
		if (ImGui::TreeNode("Effecttree4", "Gradation"))
		{
			static float s_fCustR[10];
			static float s_fCustG[10];
			static float s_fCustB[10];
			static int s_nSpeed = 1;
			static int selecttype = 0;

			ImGui::RadioButton("Custom", &selecttype, 1);

			if (selecttype == 1)
			{
				static int s_nSetTime = 0;
				static int nTypeNum = 0;
				const char *Items[] = { "Red", "Green", "Blue"};
				ImGui::Combo("ColorType", &nTypeNum, Items, IM_ARRAYSIZE(Items));

				//赤
				if (nTypeNum == 0)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustR, IM_ARRAYSIZE(s_fCustR), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::SliderFloat("Red", &s_fCustR[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				//緑
				if (nTypeNum == 1)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustG, IM_ARRAYSIZE(s_fCustG), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::SliderFloat("Green", &s_fCustG[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				//青
				if (nTypeNum == 2)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustB, IM_ARRAYSIZE(s_fCustB), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.6f, 1.0f, 1.0f));
					ImGui::SliderFloat("Blue", &s_fCustB[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				ImGui::SliderInt("SetKey", &s_nSetTime, 0, 9);
				ImGui::SliderInt("Speed", &s_nSpeed, 1, 30);		//数値が高くなると変化速度がゆっくりになる

				/*グラフの全ての色の数値を０にする*/
				if (ImGui::Button("All Zero"))
				{
					for (int i = 0; i < 10; i++)
					{
						memset(&s_fCustR[i], 0, sizeof(s_fCustR[i]));
						memset(&s_fCustG[i], 0, sizeof(s_fCustG[i]));
						memset(&s_fCustB[i], 0, sizeof(s_fCustB[i]));
					}
				}
			}

			ImGui::RadioButton("Gradation None", &selecttype, 0);

			static int s_nCounter;
			static int s_nTimer;
			static int s_nColNum;

			switch (selecttype)
			{
			case 1:
				s_nCounter++;

				//ゼロ除算回避
				if (s_nSpeed <= 0)
				{
					s_nSpeed = 1;
				}

				if ((s_nCounter % s_nSpeed) == 0)
				{//一定時間経過
					s_nTimer++;

					if (s_nTimer >= 5)
					{
						imguiParticle.particle.color.colTransition = D3DXCOLOR(s_fCustR[s_nColNum], s_fCustG[s_nColNum], s_fCustB[s_nColNum], 0.0f);
						s_nColNum++;
						s_nTimer = 0;
					}
				}

				if (s_nCounter >= 60)
				{
					s_nCounter = 0;
				}

				if (s_nColNum >= 10)
				{
					s_nColNum = 0;
				}

				break;

			case 2:
				
				break;
			case 0:
				break;
			default:
				break;
			}

			ImGui::SliderFloat("Alpha", &imguiParticle.particle.color.colTransition.a, -0.5f, 0.0f);

			ImGui::TreePop();
		}

		// αブレンディングの種類
		if (ImGui::TreeNode("Effecttree5", "AlphaBlending"))
		{
			// 変数宣言
			int	nBlendingType = (int)imguiParticle.particle.alphaBlend;		// 種別変更用の変数

			ImGui::RadioButton("AddBlend", &nBlendingType, 0);
			ImGui::RadioButton("SubBlend", &nBlendingType, 1);
			ImGui::RadioButton("BlendNone", &nBlendingType, 2);

			imguiParticle.particle.alphaBlend = (CParticle::ALPHABLENDTYPE)nBlendingType;

			//ツリーを閉じる
			ImGui::TreePop();
		}

		//ツリーを閉じる
		ImGui::TreePop();
	}



	ImPlot::ShowDemoWindow();

	ImGui::End();

#endif // _DEBUG
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawImguiProperty(void)
{
#ifdef _DEBUG

	if (!s_window)
	{// ウインドウを使用しない
		return;
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

#endif // _DEBUG
}

//--------------------------------------------------
// Imguiの取得
//--------------------------------------------------
FileParticleData& GetImguiParticle(void)
{
	return imguiParticle;
}

//--------------------------------------------------
// ファイル名の取得
//--------------------------------------------------
char* GetFileName(void)
{
	return FileString;
}

//--------------------------------------------------
// ファイル名の取得
//--------------------------------------------------
void SetFileName(char*FileStringData)
{
	strcpy(FileString, FileStringData);
}

//--------------------------------------------------
// エフェクト使用状況の取得
//--------------------------------------------------
bool bSetImguiParticle(void)
{
	return s_bEffectEnable;
}

