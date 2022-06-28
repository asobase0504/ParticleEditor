// MIT License

// Copyright (c) 2021 Evan Pezent

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// ImPlot v0.13 WIP

// We define this so that the demo does not accidentally use deprecated API
#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

#include "implot.h"
#include "imgui.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file.h"
#include "letter.h"

namespace nl = nlohmann;

nl::json Imgui;//リストの生成
#ifdef _MSC_VER
#define sprintf sprintf_s
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

// Encapsulates examples for customizing ImPlot.
namespace MyImPlot {

// Example for Custom Data and Getters section.
struct Vector2f {
    Vector2f(float _x, float _y) { x = _x; y = _y; }
    float x, y;
};

// Example for Custom Data and Getters section.
struct WaveData {
    double X, Amp, Freq, Offset;
    WaveData(double x, double amp, double freq, double offset) { X = x; Amp = amp; Freq = freq; Offset = offset; }
};
ImPlotPoint SineWave(void* wave_data, int idx);
ImPlotPoint SawWave(void* wave_data, int idx);
ImPlotPoint Spiral(void*, int idx);

// Example for Custom Plotters and Tooltips section.
void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0,1,0,1), ImVec4 bearCol = ImVec4(1,0,0,1));

} // namespace MyImPlot

namespace ImPlot {

void ShowBenchmarkTool();

template <typename T>
inline T RandomRange(T min, T max) {
    T scale = rand() / (T) RAND_MAX;
    return min + scale * ( max - min );
}

ImVec4 RandomColor() {
    ImVec4 col;
    col.x = RandomRange(0.0f,1.0f);
    col.y = RandomRange(0.0f,1.0f);
    col.z = RandomRange(0.0f,1.0f);
    col.w = 1.0f;
    return col;
}

double RandomGauss() {
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	return X;
}

template <int N>
struct NormalDistribution {
    NormalDistribution(double mean, double sd) {
        for (int i = 0; i < N; ++i)
            Data[i] = RandomGauss()*sd + mean;
    }
    double Data[N];
};

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
};

// utility structure for realtime plot
struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

void ButtonSelector(const char* label, ImGuiMouseButton* b) {
    ImGui::PushID(label);
    if (ImGui::RadioButton("LMB",*b == ImGuiMouseButton_Left))
        *b = ImGuiMouseButton_Left;
    ImGui::SameLine();
    if (ImGui::RadioButton("RMB",*b == ImGuiMouseButton_Right))
        *b = ImGuiMouseButton_Right;
    ImGui::SameLine();
    if (ImGui::RadioButton("MMB",*b == ImGuiMouseButton_Middle))
        *b = ImGuiMouseButton_Middle;
    ImGui::PopID();
}

void ModSelector(const char* label, ImGuiModFlags* k) {
    ImGui::PushID(label);
    ImGui::CheckboxFlags("Ctrl", (unsigned int*)k, ImGuiModFlags_Ctrl); ImGui::SameLine();
    ImGui::CheckboxFlags("Shift", (unsigned int*)k, ImGuiModFlags_Shift); ImGui::SameLine();
    ImGui::CheckboxFlags("Alt", (unsigned int*)k, ImGuiModFlags_Alt); ImGui::SameLine();
    ImGui::CheckboxFlags("Super", (unsigned int*)k, ImGuiModFlags_Super);
    ImGui::PopID();
}

void InputMapping(const char* label, ImGuiMouseButton* b, ImGuiModFlags* k) {
    ImGui::LabelText("##","%s",label);
    if (b != NULL) {
        ImGui::SameLine(100);
        ButtonSelector(label,b);
    }
    if (k != NULL) {
        ImGui::SameLine(300);
        ModSelector(label,k);
    }
}

//-----------------------------------------------------------------------------

void ShowDemo_LinePlots() {
    static float xs1[1001], ys1[1001];
    for (int i = 0; i < 1001; ++i) {
        xs1[i] = i * 0.001f;
        ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
    }
    static double xs2[11], ys2[11];
    for (int i = 0; i < 11; ++i) {
        xs2[i] = i * 0.1f;
        ys2[i] = xs2[i] * xs2[i];
    }
    ImGui::BulletText("Anti-aliasing can be enabled from the plot's context menu (see Help).");
    if (ImPlot::BeginPlot("Line Plot")) {
        ImPlot::SetupAxes("x","f(x)");
        ImPlot::PlotLine("sin(x)", xs1, ys1, 1001);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine("x^2", xs2, ys2, 11);
        ImPlot::EndPlot();
    }
}

void ShowDemo_StairstepPlots() {
    static float ys1[101], ys2[101];
    for (int i = 0; i < 101; ++i) {
        ys1[i] = 0.5f + 0.4f * sinf(50 * i * 0.01f);
        ys2[i] = 0.5f + 0.2f * sinf(25 * i * 0.01f);
    }
    if (ImPlot::BeginPlot("Stairstep Plot")) {
        ImPlot::SetupAxes("x","f(x)");
        ImPlot::PlotStairs("Signal 1", ys1, 101, 0.01f);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 2.0f);
        ImPlot::PlotStairs("Signal 2", ys2, 101, 0.01f);
        ImPlot::EndPlot();
    }
}

void ShowDemo_LogAxes() {
    static double xs[1001], ys1[1001], ys2[1001], ys3[1001];
    for (int i = 0; i < 1001; ++i) {
        xs[i]  = i*0.1f;
        ys1[i] = sin(xs[i]) + 1;
        ys2[i] = log(xs[i]);
        ys3[i] = pow(10.0, xs[i]);
    }
    ImGui::BulletText("Open the plot context menu (right click) to change scales.");

    if (ImPlot::BeginPlot("Log Plot", ImVec2(-1,0))) {
        ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_LogScale);
        ImPlot::SetupAxesLimits(0.1, 100, 0, 10);
        ImPlot::PlotLine("f(x) = x",        xs, xs,  1001);
        ImPlot::PlotLine("f(x) = sin(x)+1", xs, ys1, 1001);
        ImPlot::PlotLine("f(x) = log(x)",   xs, ys2, 1001);
        ImPlot::PlotLine("f(x) = 10^x",     xs, ys3, 21);
        ImPlot::EndPlot();
    }
}

void ShowDemo_LinkedAxes() {
    static ImPlotRect lims(0,1,0,1);
    static bool linkx = true, linky = true;
    int data[2] = {0,1};
    ImGui::Checkbox("Link X", &linkx);
    ImGui::SameLine();
    ImGui::Checkbox("Link Y", &linky);

    ImGui::DragScalarN("Limits",ImGuiDataType_Double,&lims.X.Min,4,0.01f);

    if (BeginAlignedPlots("AlignedGroup")) {
        if (ImPlot::BeginPlot("Plot A")) {
            ImPlot::SetupAxisLinks(ImAxis_X1, linkx ? &lims.X.Min : NULL, linkx ? &lims.X.Max : NULL);
            ImPlot::SetupAxisLinks(ImAxis_Y1, linky ? &lims.Y.Min : NULL, linky ? &lims.Y.Max : NULL);
            ImPlot::PlotLine("Line",data,2);
            ImPlot::EndPlot();
        }
        if (ImPlot::BeginPlot("Plot B")) {
            ImPlot::SetupAxisLinks(ImAxis_X1, linkx ? &lims.X.Min : NULL, linkx ? &lims.X.Max : NULL);
            ImPlot::SetupAxisLinks(ImAxis_Y1, linky ? &lims.Y.Min : NULL, linky ? &lims.Y.Max : NULL);
            ImPlot::PlotLine("Line",data,2);
            ImPlot::EndPlot();
        }
        ImPlot::EndAlignedPlots();
    }
}

void ShowDemo_EqualAxes() {
    ImGui::BulletText("Equal constraint applies to axis pairs (e.g ImAxis_X1/Y1, ImAxis_X2/Y2");
    static double xs1[360], ys1[360];
    for (int i = 0; i < 360; ++i) {
        double angle = i * 2 * PI / 359.0;
        xs1[i] = cos(angle); ys1[i] = sin(angle);
    }
    float xs2[] = {-1,0,1,0,-1};
    float ys2[] = {0,1,0,-1,0};
    if (ImPlot::BeginPlot("##EqualAxes",ImVec2(-1,0),ImPlotFlags_Equal)) {
        ImPlot::SetupAxis(ImAxis_X2, NULL, ImPlotAxisFlags_AuxDefault);
        ImPlot::SetupAxis(ImAxis_Y2, NULL, ImPlotAxisFlags_AuxDefault);
        ImPlot::PlotLine("Circle",xs1,ys1,360);
        ImPlot::SetAxes(ImAxis_X2, ImAxis_Y2);
        ImPlot::PlotLine("Diamond",xs2,ys2,5);
        ImPlot::EndPlot();
    }
}

ImPlotPoint SinewaveGetter(void* data, int i) {
    float f = *(float*)data;
    return ImPlotPoint(i,sinf(f*i));
}

void ShowDemo_SubplotsSizing() {

    static ImPlotSubplotFlags flags = ImPlotSubplotFlags_None;
    ImGui::CheckboxFlags("ImPlotSubplotFlags_NoResize", (unsigned int*)&flags, ImPlotSubplotFlags_NoResize);
    ImGui::CheckboxFlags("ImPlotSubplotFlags_NoTitle", (unsigned int*)&flags, ImPlotSubplotFlags_NoTitle);

    static int rows = 3;
    static int cols = 3;
    ImGui::SliderInt("Rows",&rows,1,5);
    ImGui::SliderInt("Cols",&cols,1,5);
    static float rratios[] = {5,1,1,1,1,1};
    static float cratios[] = {5,1,1,1,1,1};
    ImGui::DragScalarN("Row Ratios",ImGuiDataType_Float,rratios,rows,0.01f,0);
    ImGui::DragScalarN("Col Ratios",ImGuiDataType_Float,cratios,cols,0.01f,0);
    if (ImPlot::BeginSubplots("My Subplots", rows, cols, ImVec2(-1,400), flags, rratios, cratios)) {
        for (int i = 0; i < rows*cols; ++i) {
            if (ImPlot::BeginPlot("",ImVec2(),ImPlotFlags_NoLegend)) {
                ImPlot::SetupAxes(NULL,NULL,ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
                float fi = 0.01f * (i+1);
                ImPlot::SetNextLineStyle(SampleColormap((float)i/(float)(rows*cols-1),ImPlotColormap_Jet));
                ImPlot::PlotLineG("data",SinewaveGetter,&fi,1000);
                ImPlot::EndPlot();
            }
        }
        ImPlot::EndSubplots();
    }
}

void ShowDemo_SubplotItemSharing() {
    static ImPlotSubplotFlags flags = ImPlotSubplotFlags_ShareItems;
    ImGui::CheckboxFlags("ImPlotSubplotFlags_ShareItems", (unsigned int*)&flags, ImPlotSubplotFlags_ShareItems);
    ImGui::CheckboxFlags("ImPlotSubplotFlags_ColMajor", (unsigned int*)&flags, ImPlotSubplotFlags_ColMajor);
    ImGui::BulletText("Drag and drop items from the legend onto plots (except for 'common')");
    static int rows = 2;
    static int cols = 3;
    static int id[] = {0,1,2,3,4,5};
    static int curj = -1;
    if (ImPlot::BeginSubplots("##ItemSharing", rows, cols, ImVec2(-1,400), flags)) {
        for (int i = 0; i < rows*cols; ++i) {
            if (ImPlot::BeginPlot("")) {
                float fc = 0.01f;
                ImPlot::PlotLineG("common",SinewaveGetter,&fc,1000);
                for (int j = 0; j < 6; ++j) {
                    if (id[j] == i) {
                        char label[8];
                        float fj = 0.01f * (j+2);
                        sprintf(label, "data%d", j);
                        ImPlot::PlotLineG(label,SinewaveGetter,&fj,1000);
                        if (ImPlot::BeginDragDropSourceItem(label)) {
                            curj = j;
                            ImGui::SetDragDropPayload("MY_DND",NULL,0);
                            ImPlot::ItemIcon(GetLastItemColor()); ImGui::SameLine();
                            ImGui::TextUnformatted(label);
                            ImPlot::EndDragDropSource();
                        }
                    }
                }
                if (ImPlot::BeginDragDropTargetPlot()) {
                    if (ImGui::AcceptDragDropPayload("MY_DND"))
                        id[curj] = i;
                    ImPlot::EndDragDropTarget();
                }
                ImPlot::EndPlot();
            }
        }
        ImPlot::EndSubplots();
    }
}

ImPlotPoint FindCentroid(const ImVector<ImPlotPoint>& data, ImPlotRect& bounds, int& cnt) {
    cnt = 0;
    ImPlotPoint avg;
    for (int i = 0; i < data.size(); ++i) {
        if (bounds.Contains(data[i].x, data[i].y)) {
            avg.x += data[i].x;
            avg.y += data[i].y;
            cnt++;
        }
    }
    if (cnt > 0) {
        avg.x = avg.x / cnt;
        avg.y = avg.y / cnt;
    }
    return avg;
}

/*ImPlot研究用のプログラム*/
void ShowDemo_Querying() 
{
    static ImVector<ImPlotPoint> data;
    static ImVector<ImPlotRect> rects;
    static ImPlotRect limits, select;
    static bool init = true;

    if (init) 
	{
        for (int i = 0; i < 1; i++)
        {
            double x = RandomRange(0.0, 0.0);
            double y = RandomRange(0.0, 0.0);
            data.push_back(ImPlotPoint(x,y));
        }
        init = false;
    }

    if (ImGui::Button("Clear Queries"))
        rects.shrink(0);

	static ScrollingBuffer sdata1, sdata2;
	static RollingBuffer   rdata1, rdata2;
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotPoint pt;
	ImVec2 mouse = ImGui::GetMousePos();
	ImVec2 ptPos[512] = {};
	static bool s_bPlay = false;
	static bool s_bTag = false;
	static bool s_bLoop = false;
	static int s_nIndex = 0;
	static int s_nSaveIdx = 0;
	static int s_nSelectPt = 0;
	static float flt = 0.0f;
	static float s_fStartTimeA = rdata1.Span;
	static float s_fStartTimeB = rdata2.Span;
	static float s_fStopTime = 0.0f;
	static float s_History = 5.0f;
	static float t = 0;
	static double s_dMin = 0.0f;
	static double s_dMax = 10.0f;

	//データ出力
	if (ImGui::Button("DataOutPut"))
	{
		for (int nCnt = 0; nCnt < s_nIndex; nCnt++)
		{
			Imgui["Index"] = s_nIndex;
			std::string name = "ANIME";
			std::string Number = std::to_string(nCnt);
			name += Number;
			Imgui[name] = { { "X", data[nCnt].x } ,{ "Y", data[nCnt].y } };

		}
		auto jobj = Imgui.dump();
		std::ofstream writing_file;
		const std::string pathToJSON = "data/FILE/Animetest.json";
		writing_file.open(pathToJSON, std::ios::out);
		writing_file << jobj << std::endl;
		writing_file.close();
	}
	if (ImGui::Button("DataLoad"))
	{
		//ロードする前にロードする前の点を全て消す
		for (int i = 0; i < s_nSaveIdx; i++)
		{
			s_nIndex--;
			data.pop_back();
		}

		const std::string pathToJSON = "data/FILE/Animetest.json";
		std::ifstream ifs(pathToJSON);

		if (ifs)
		{
			//StringToWString(UTF8toSjis(j["name"]));
			//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
			ifs >> Imgui;
			s_nIndex = Imgui["Index"];
			for (int nCnt = 0; nCnt < s_nIndex; nCnt++)
			{
				
				std::string name = "ANIME";
				std::string Number = std::to_string(nCnt);
				name += Number;

				ImVec2 Pos = ImVec2(Imgui[name]["X"], Imgui[name]["Y"]);

				data.push_back(Pos);
			}
		}	
	}

	ImGui::Text("Ctrl + Left Click : Set point");
	ImGui::Text("Del + Left Click : Delete point");
	ImGui::Text("Double Left Click : Fit");

	//グラフを再生させる
	if (ImGui::Checkbox("Play", &s_bPlay))
	{
		rdata1.Span = s_History;
		rdata2.Span = s_History;
	}

	//グラフを再生して変化した数値を初期値に戻す
	ImGui::SameLine();
	if (ImGui::Button("Init Val"))
	{
		t = 0;
		s_fStopTime = 0.0f;
		s_dMin = 0.0f;
		s_dMax = 10.0f;
		rdata1.Span = s_fStartTimeA;
		rdata2.Span = s_fStartTimeB;
	}

	ImGui::Checkbox("Loop", &s_bLoop);

	ImGui::SameLine();
	ImGui::Checkbox("Tag", &s_bTag);

	ImGui::Text("Selected Point : %d", s_nSelectPt);
	ImGui::InputDouble("Select Key", &data[s_nSelectPt].x);
	ImGui::InputDouble("Select Value", &data[s_nSelectPt].y);

	if (s_bTag)
	{//タグが表示された場合
		ImGui::InputDouble("TagMin", &s_dMin, 0.1f);
		ImGui::InputDouble("TagMax", &s_dMax, 0.1f);
	}

	if (ImGui::Button("Remove"))
	{
		for (int i = 0; i < s_nSaveIdx; i++)
		{
			s_nIndex--;
			data.pop_back();
		}
	}

    if (ImPlot::BeginPlot("Timeline")) 
	{
		static ImPlotPoint point[] = {ImPlotPoint(0.2f,0.4f), ImPlotPoint(0.95f,0.95f) };
		ImPlot::SetupAxes(0, 0, flags, flags);
        ImPlot::SetupAxesLimits(0,1,0,1);
		s_nSaveIdx = s_nIndex;

		if (s_bPlay)
		{
			t += ImGui::GetIO().DeltaTime;

			//再生中のグラフの拡大
			ImGui::SliderFloat("History", &s_History, 1, 30, "%.1f s");

			ImPlot::SetupAxisLimits(ImAxis_X1, t - s_History, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

			//最後の点に到達したらまた最初から開始
			if (s_bLoop)
			{
				if (data[s_nIndex].x < t)
				{
					t = 0;
				}
			}
		}

		//点置くところ
        if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl)
		{
			pt = ImPlot::GetPlotMousePos();

			//タグの数値以下（以上）には点を設置できない
			if (s_bTag)
			{
				if (pt.x < s_dMin || pt.x > s_dMax)
				{
				}

				else
				{
					data.push_back(pt);
					s_nIndex++;
				}
			}

			else
			{
				data.push_back(pt);
				s_nIndex++;
			}
        }

		//点の位置のソート
		for (int i = 0; i < s_nIndex; i++)
		{
			for (int j = i; j < s_nIndex + 1; j++)
			{
				if (data[i].x > data[j].x)
				{
					ImPlotPoint Sort = data[i];
					data[i] = data[j];
					data[j] = Sort;

					//点の位置の数値を保存
					ptPos[i] = ImVec2((float)data[i].x, (float)data[i].y);
				}
			}
		}

		//点動かす用
		for (int i = 0; i < s_nIndex + 1; i++)
		{
			ImPlot::DragPoint(s_nIndex + i,&data[i].x, &data[i].y, ImVec4(0, 0.9f, 0, 1), 4, flags);
		}

		//線引く用
        ImPlot::PlotScatter("Points", &data[0].x, &data[0].y, data.size(), s_nIndex, 2 * sizeof(double));
		ImPlot::PlotLine("##Line", &data[0].x, &data[0].y, data.size(), s_nIndex + 1, 2 * sizeof(double));

		//タグの表示
		if (s_bTag)
		{
			ImPlot::TagX(s_dMin, ImVec4(1, 1, 0, 1));
			ImPlot::DragLineX(0, &s_dMin, ImVec4(1, 0, 0, 1), 1, ImPlotDragToolFlags_NoFit);

			ImPlot::TagX(s_dMax, ImVec4(1, 0, 1, 1));
			ImPlot::DragLineX(1, &s_dMax, ImVec4(1, 0, 0, 1), 1, ImPlotDragToolFlags_NoFit);
		}

		//右クリックの範囲選択
        if (ImPlot::IsPlotSelected())
		{
            select = ImPlot::GetPlotSelection();

            int cnt;
			int ptNum = 0;
			pt = FindCentroid(data, select, cnt);

			//点の番号を取る
			for (int i = 0; i < data.size(); ++i) 
			{
				if (select.Contains(data[i].x, data[i].y)) 
				{
					//点を削除
					if (ImGui::IsMouseClicked(0) && ImGui::GetIO().KeysDown[ImGuiKey_Delete])
					{//Deleteを押下したまま、クリックした場合
						if (s_nIndex > 0)
						{
							//選択した１点のみを削除
							for (int j = i; j < s_nIndex; j++)
							{
								data[j] = data[j + 1];
							}

							data.pop_back();
							s_nIndex--;
						}
					}

					ptNum++;
					s_nSelectPt = i;
				}
			}

			//選択範囲の中の点をとる
            if (cnt > 0)
			{
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Square,6);

                ImPlot::PlotScatter("Centroid", &pt.x, &pt.y, 1);
            }

   //         if (ImGui::IsMouseClicked(ImPlot::GetInputMap().SelectCancel)) 
			//{
   //             CancelPlotSelection();
   //             rects.push_back(select);
   //         }
        }

        for (int i = 0; i < rects.size(); ++i) 
		{
            int cnt;
            ImPlotPoint centroid = FindCentroid(data,rects[i],cnt);

            if (cnt > 0) {
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Square,6);
                ImPlot::PlotScatter("Centroid", &centroid.x, &centroid.y, 1);
            }

			ImPlot::DragRect(i, &rects[i].X.Min, &rects[i].Y.Min, &rects[i].X.Max, &rects[i].Y.Max, ImVec4(1, 0, 1, 1));
        }

        limits  = ImPlot::GetPlotLimits();
        ImPlot::EndPlot();
    }
}

void MetricFormatter(double value, char* buff, int size, void* data) {
    const char* unit = (const char*)data;
    static double v[]      = {1000000000,1000000,1000,1,0.001,0.000001,0.000000001};
    static const char* p[] = {"G","M","k","","m","u","n"};
    if (value == 0) {
        snprintf(buff,size,"0 %s", unit);
        return;
    }
    for (int i = 0; i < 7; ++i) {
        if (fabs(value) >= v[i]) {
            snprintf(buff,size,"%g %s%s",value/v[i],p[i],unit);
            return;
        }
    }
    snprintf(buff,size,"%g %s%s",value/v[6],p[6],unit);
}

//-----------------------------------------------------------------------------
// DEMO WINDOW
//-----------------------------------------------------------------------------

void ShowDemoWindow(bool* p_open) 
{
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
    ImGui::Begin("ImPlot Demo", p_open, ImGuiWindowFlags_MenuBar);

    //-------------------------------------------------------------------------
    ImGui::Text("ImPlot says hello. (%s)", IMPLOT_VERSION);
    // display warning about 16-bit indices
    static bool showWarning = sizeof(ImDrawIdx)*8 == 16 && (ImGui::GetIO().BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) == false;
    if (showWarning) 
	{
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,0,1));
        ImGui::TextWrapped("WARNING: ImDrawIdx is 16-bit and ImGuiBackendFlags_RendererHasVtxOffset is false. Expect visual glitches and artifacts! See README for more information.");
        ImGui::PopStyleColor();
    }

    ImGui::Spacing();

    if (ImGui::BeginTabBar("ImPlotDemoTabs"))
	{
       //if (ImGui::BeginTabItem("Plots")) {
		   
       //     if (ImGui::CollapsingHeader("Line Plots"))
       //         ShowDemo_LinePlots();
       //     if (ImGui::CollapsingHeader("Stairstep Plots"))
       //         ShowDemo_StairstepPlots();
       //     ImGui::EndTabItem();
       // }

   //     if (ImGui::BeginTabItem("Subplots")) 
   //     {
   //         if (ImGui::CollapsingHeader("Sizing"))
   //             ShowDemo_SubplotsSizing();
   //         if (ImGui::CollapsingHeader("Item Sharing"))
   //             ShowDemo_SubplotItemSharing();
   //         ImGui::EndTabItem();
   //     }

  //      if (ImGui::BeginTabItem("Axes")) 
		//{
  //          if (ImGui::CollapsingHeader("Log Axes"))
  //              ShowDemo_LogAxes();
  //          if (ImGui::CollapsingHeader("Linked Axes"))
  //              ShowDemo_LinkedAxes();
  //          if (ImGui::CollapsingHeader("Equal Axes"))
  //              ShowDemo_EqualAxes();
  //          ImGui::EndTabItem();
  //      }

        if (ImGui::BeginTabItem("Tools")) 
		{
            if (ImGui::CollapsingHeader("Querying"))
                ShowDemo_Querying();

            ImGui::EndTabItem();
        }

     /*   if (ImGui::BeginTabItem("Config")) {
            ShowDemo_Config();
            ImGui::EndTabItem();
        }*/
        ImGui::EndTabBar();
    }
    ImGui::End();
}

} // namespace ImPlot

namespace MyImPlot {

ImPlotPoint SineWave(void* data , int idx) {
    WaveData* wd = (WaveData*)data;
    double x = idx * wd->X;
    return ImPlotPoint(x, wd->Offset + wd->Amp * sin(2 * 3.14 * wd->Freq * x));
}

ImPlotPoint SawWave(void* data, int idx) {
    WaveData* wd = (WaveData*)data;
    double x = idx * wd->X;
    return ImPlotPoint(x, wd->Offset + wd->Amp * (-2 / 3.14 * atan(cos(3.14 * wd->Freq * x) / sin(3.14 * wd->Freq * x))));
}

ImPlotPoint Spiral(void*, int idx) {
    float r = 0.9f;            // outer radius
    float a = 0;               // inner radius
    float b = 0.05f;           // increment per rev
    float n = (r - a) / b;     // number  of revolutions
    double th = 2 * n * 3.14;  // angle
    float Th = float(th * idx / (1000 - 1));
    return ImPlotPoint(0.5f+(a + b*Th / (2.0f * (float) 3.14))*cos(Th),
                       0.5f + (a + b*Th / (2.0f * (float)3.14))*sin(Th));
}

} // namespaece MyImPlot