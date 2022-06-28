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

// Example for Tables section.
void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size);

// Example for Custom Plotters and Tooltips section.
void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0,1,0,1), ImVec4 bearCol = ImVec4(1,0,0,1));

// Example for Custom Styles section.
void StyleSeaborn();

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

// Huge data used by Time Formatting example (~500 MB allocation!)
struct HugeTimeData {
    HugeTimeData(double min) {
        Ts = new double[Size];
        Ys = new double[Size];
        for (int i = 0; i < Size; ++i) {
            Ts[i] = min + i;
            Ys[i] = GetY(Ts[i]);
        }
    }
    ~HugeTimeData() { delete[] Ts;  delete[] Ys; }
    static double GetY(double t) {
        return 0.5 + 0.25 * sin(t/86400/12) +  0.005 * sin(t/3600);
    }
    double* Ts;
    double* Ys;
    static const int Size = 60*60*24*366;
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

void ShowInputMapping() {
    ImPlotInputMap& map = ImPlot::GetInputMap();
    InputMapping("Pan",&map.Pan,&map.PanMod);
    InputMapping("Fit",&map.Fit,NULL);
    InputMapping("Select",&map.Select,&map.SelectMod);
    InputMapping("SelectHorzMod",NULL,&map.SelectHorzMod);
    InputMapping("SelectVertMod",NULL,&map.SelectVertMod);
    InputMapping("SelectCancel",&map.SelectCancel,NULL);
    InputMapping("Menu",&map.Menu,NULL);
    InputMapping("OverrideMod",NULL,&map.OverrideMod);
    InputMapping("ZoomMod",NULL,&map.ZoomMod);
    ImGui::SliderFloat("ZoomRate",&map.ZoomRate,-1,1);
}

void ShowDemo_Config() {
    ImGui::ShowFontSelector("Font");
    ImGui::ShowStyleSelector("ImGui Style");
    ImPlot::ShowStyleSelector("ImPlot Style");
    ImPlot::ShowColormapSelector("ImPlot Colormap");
    ImPlot::ShowInputMapSelector("Input Map");
    ImGui::Separator();
    ImGui::Checkbox("Anti-Aliased Lines", &ImPlot::GetStyle().AntiAliasedLines);
    ImGui::Checkbox("Use Local Time", &ImPlot::GetStyle().UseLocalTime);
    ImGui::Checkbox("Use ISO 8601", &ImPlot::GetStyle().UseISO8601);
    ImGui::Checkbox("Use 24 Hour Clock", &ImPlot::GetStyle().Use24HourClock);
    ImGui::Separator();
    if (ImPlot::BeginPlot("Preview")) {
        static double now = (double)time(0);
        ImPlot::SetupAxis(ImAxis_X1,NULL,ImPlotAxisFlags_Time);
        ImPlot::SetupAxisLimits(ImAxis_X1, now, now + 24*3600);
        for (int i = 0; i < 10; ++i) {
            double x[2] = {now, now + 24*3600};
            double y[2] = {0,i/9.0};
            ImGui::PushID(i);
            ImPlot::PlotLine("##Line",x,y,2);
            ImGui::PopID();
        }
        ImPlot::EndPlot();
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

void ShowDemo_BarGroups() {
    static ImS8  data[30] = {83, 67, 23, 89, 83, 78, 91, 82, 85, 90,  // midterm
                             80, 62, 56, 99, 55, 78, 88, 78, 90, 100, // final
                             80, 69, 52, 92, 72, 78, 75, 76, 89, 95}; // course

    static const char*  ilabels[]   = {"Midterm Exam","Final Exam","Course Grade"};
    static const char*  glabels[]   = {"S1","S2","S3","S4","S5","S6","S7","S8","S9","S10"};
    static const double positions[] = {0,1,2,3,4,5,6,7,8,9};

    static int items  = 3;
    static int groups = 10;
    static float size = 0.67f;

    static ImPlotBarGroupsFlags flags = 0;
    static bool horz = false;

    ImGui::CheckboxFlags("Stacked", (unsigned int*)&flags, ImPlotBarGroupsFlags_Stacked);
    ImGui::SameLine();
    ImGui::Checkbox("Horizontal",&horz);

    ImGui::SliderInt("Items",&items,1,3);
    ImGui::SliderFloat("Size",&size,0,1);

    if (ImPlot::BeginPlot("Bar Group")) {
        ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
        if (horz) {
            ImPlot::SetupAxes("Score","Student",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisTicks(ImAxis_Y1,positions, groups, glabels);
            ImPlot::PlotBarGroupsH(ilabels,data,items,groups,size,0,flags);
        }
        else {
            ImPlot::SetupAxes("Student","Score",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisTicks(ImAxis_X1,positions, groups, glabels);
            ImPlot::PlotBarGroups(ilabels,data,items,groups,size,0,flags);
        }
        ImPlot::EndPlot();
    }
}

//-----------------------------------------------------------------------------

void ShowDemo_ErrorBars() {
    static float xs[5]    = {1,2,3,4,5};
    static float bar[5]   = {1,2,5,3,4};
    static float lin1[5]  = {8,8,9,7,8};
    static float lin2[5]  = {6,7,6,9,6};
    static float err1[5]  = {0.2f, 0.4f, 0.2f, 0.6f, 0.4f};
    static float err2[5]  = {0.4f, 0.2f, 0.4f, 0.8f, 0.6f};
    static float err3[5]  = {0.09f, 0.14f, 0.09f, 0.12f, 0.16f};
    static float err4[5]  = {0.02f, 0.08f, 0.15f, 0.05f, 0.2f};


    if (ImPlot::BeginPlot("##ErrorBars")) {
        ImPlot::SetupAxesLimits(0, 6, 0, 10);
        ImPlot::PlotBars("Bar", xs, bar, 5, 0.5f);
        ImPlot::PlotErrorBars("Bar", xs, bar, err1, 5);
        ImPlot::SetNextErrorBarStyle(ImPlot::GetColormapColor(1), 0);
        ImPlot::PlotErrorBars("Line", xs, lin1, err1, err2, 5);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine("Line", xs, lin1, 5);
        ImPlot::PushStyleColor(ImPlotCol_ErrorBar, ImPlot::GetColormapColor(2));
        ImPlot::PlotErrorBars("Scatter", xs, lin2, err2, 5);
        ImPlot::PlotErrorBarsH("Scatter", xs, lin2,  err3, err4, 5);
        ImPlot::PopStyleColor();
        ImPlot::PlotScatter("Scatter", xs, lin2, 5);
        ImPlot::EndPlot();
    }
}

void ShowDemo_StemPlots() {
    static double xs[51], ys1[51], ys2[51];
    for (int i = 0; i < 51; ++i) {
        xs[i] = i * 0.02;
        ys1[i] = 1.0 + 0.5 * sin(25*xs[i])*cos(2*xs[i]);
        ys2[i] = 0.5 + 0.25  * sin(10*xs[i]) * sin(xs[i]);
    }
    if (ImPlot::BeginPlot("Stem Plots")) {
        ImPlot::SetupAxisLimits(ImAxis_X1,0,1.0);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0,1.6);
        ImPlot::PlotStems("Stems 1",xs,ys1,51);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square,5);
        ImPlot::PlotStems("Stems 2", xs, ys2,51);
        ImPlot::EndPlot();
    }
}

void ShowDemo_InfiniteLines() {
    static double vals[] = {0.25, 0.5, 0.75};
    if (ImPlot::BeginPlot("##Infinite")) {
        ImPlot::SetupAxes(NULL,NULL,ImPlotAxisFlags_NoInitialFit,ImPlotAxisFlags_NoInitialFit);
        ImPlot::PlotVLines("VLines",vals,3);
        ImPlot::PlotHLines("HLines",vals,3);
        ImPlot::EndPlot();
    }
}

void ShowDemo_Histogram() {
    static int  bins       = 50;
    static bool cumulative = false;
    static bool density    = true;
    static bool outliers   = true;
    static double mu       = 5;
    static double sigma    = 2;

    ImGui::SetNextItemWidth(200);
    if (ImGui::RadioButton("Sqrt",bins==ImPlotBin_Sqrt))       { bins = ImPlotBin_Sqrt;    } ImGui::SameLine();
    if (ImGui::RadioButton("Sturges",bins==ImPlotBin_Sturges)) { bins = ImPlotBin_Sturges; } ImGui::SameLine();
    if (ImGui::RadioButton("Rice",bins==ImPlotBin_Rice))       { bins = ImPlotBin_Rice;    } ImGui::SameLine();
    if (ImGui::RadioButton("Scott",bins==ImPlotBin_Scott))     { bins = ImPlotBin_Scott;   } ImGui::SameLine();
    if (ImGui::RadioButton("N Bins",bins>=0))                       bins = 50;
    if (bins>=0) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##Bins", &bins, 1, 100);
    }
    if (ImGui::Checkbox("Density", &density))
    {
        ImPlot::SetNextAxisToFit(ImAxis_X1);
        ImPlot::SetNextAxisToFit(ImAxis_Y1);
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Cumulative", &cumulative))
    {
        ImPlot::SetNextAxisToFit(ImAxis_X1);
        ImPlot::SetNextAxisToFit(ImAxis_Y1);
    }
    ImGui::SameLine();
    static bool range = false;
    ImGui::Checkbox("Range", &range);
    static float rmin = -3;
    static float rmax = 13;
    if (range) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::DragFloat2("##Range",&rmin,0.1f,-3,13);
        ImGui::SameLine();
        ImGui::Checkbox("Outliers",&outliers);
    }

    static NormalDistribution<10000> dist(mu, sigma);
    static double x[100];
    static double y[100];
    if (density) {
        for (int i = 0; i < 100; ++i) {
            x[i] = -3 + 16 * (double)i/99.0;
            y[i] = exp( - (x[i]-mu)*(x[i]-mu) / (2*sigma*sigma)) / (sigma * sqrt(2*3.141592653589793238));
        }
        if (cumulative) {
            for (int i = 1; i < 100; ++i)
                y[i] += y[i-1];
            for (int i = 0; i < 100; ++i)
                y[i] /= y[99];
        }
    }

    if (ImPlot::BeginPlot("##Histograms")) {
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        ImPlot::PlotHistogram("Empirical", dist.Data, 10000, bins, cumulative, density, range ? ImPlotRange(rmin,rmax) : ImPlotRange(), outliers);
        if (density && outliers)
            ImPlot::PlotLine("Theoretical",x,y,100);
        ImPlot::EndPlot();
    }
}

void ShowDemo_DigitalPlots() {
    ImGui::BulletText("Digital plots do not respond to Y drag and zoom, so that");
    ImGui::Indent();
    ImGui::Text("you can drag analog plots over the rising/falling digital edge.");
    ImGui::Unindent();

    static bool paused = false;
    static ScrollingBuffer dataDigital[2];
    static ScrollingBuffer dataAnalog[2];
    static bool showDigital[2] = {true, false};
    static bool showAnalog[2] = {true, false};

    char label[32];
    ImGui::Checkbox("digital_0", &showDigital[0]); ImGui::SameLine();
    ImGui::Checkbox("digital_1", &showDigital[1]); ImGui::SameLine();
    ImGui::Checkbox("analog_0",  &showAnalog[0]);  ImGui::SameLine();
    ImGui::Checkbox("analog_1",  &showAnalog[1]);

    static float t = 0;
    if (!paused) {
        t += ImGui::GetIO().DeltaTime;
        //digital signal values
        if (showDigital[0])
            dataDigital[0].AddPoint(t, sinf(2*t) > 0.45);
        if (showDigital[1])
            dataDigital[1].AddPoint(t, sinf(2*t) < 0.45);
        //Analog signal values
        if (showAnalog[0])
            dataAnalog[0].AddPoint(t, sinf(2*t));
        if (showAnalog[1])
            dataAnalog[1].AddPoint(t, cosf(2*t));
    }
    if (ImPlot::BeginPlot("##Digital")) {
        ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
        for (int i = 0; i < 2; ++i) {
            if (showDigital[i] && dataDigital[i].Data.size() > 0) {
                sprintf(label, "digital_%d", i);
                ImPlot::PlotDigital(label, &dataDigital[i].Data[0].x, &dataDigital[i].Data[0].y, dataDigital[i].Data.size(), dataDigital[i].Offset, 2 * sizeof(float));
            }
        }
        for (int i = 0; i < 2; ++i) {
            if (showAnalog[i]) {
                sprintf(label, "analog_%d", i);
                if (dataAnalog[i].Data.size() > 0)
                    ImPlot::PlotLine(label, &dataAnalog[i].Data[0].x, &dataAnalog[i].Data[0].y, dataAnalog[i].Data.size(), dataAnalog[i].Offset, 2 * sizeof(float));
            }
        }
        ImPlot::EndPlot();
    }
}

//void ShowDemo_RealtimePlots() {
//    ImGui::BulletText("Move your mouse to change the data!");
//    ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
//    static ScrollingBuffer sdata1, sdata2;
//    static RollingBuffer   rdata1, rdata2;
//    ImVec2 mouse = ImGui::GetMousePos();
//    static float t = 0;
//    t += ImGui::GetIO().DeltaTime;
//    sdata1.AddPoint(t, mouse.x * 0.0005f);
//    rdata1.AddPoint(t, mouse.x * 0.0005f);
//    sdata2.AddPoint(t, mouse.y * 0.0005f);
//    rdata2.AddPoint(t, mouse.y * 0.0005f);
//
//    static float history = 10.0f;
//    ImGui::SliderFloat("History",&history,1,30,"%.1f s");
//    rdata1.Span = history;
//    rdata2.Span = history;
//
//    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
//
//    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
//        ImPlot::SetupAxes(NULL, NULL, flags, flags);
//        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
//        ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
//        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
//        ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, sdata1.Offset, 2 * sizeof(float));
//        ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2*sizeof(float));
//        ImPlot::EndPlot();
//    }
//    if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,150))) {
//        ImPlot::SetupAxes(NULL, NULL, flags, flags);
//        ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
//        ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
//        ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 2 * sizeof(float));
//        ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 2 * sizeof(float));
//        ImPlot::EndPlot();
//    }
//}

void ShowDemo_MarkersAndText() {
    static float mk_size = ImPlot::GetStyle().MarkerSize;
    static float mk_weight = ImPlot::GetStyle().MarkerWeight;
    ImGui::DragFloat("Marker Size",&mk_size,0.1f,2.0f,10.0f,"%.2f px");
    ImGui::DragFloat("Marker Weight", &mk_weight,0.05f,0.5f,3.0f,"%.2f px");

    if (ImPlot::BeginPlot("##MarkerStyles", ImVec2(-1,0), ImPlotFlags_CanvasOnly)) {

        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxesLimits(0, 10, 0, 12);

        ImS8 xs[2] = {1,4};
        ImS8 ys[2] = {10,11};

        // filled markers
        for (int m = 0; m < ImPlotMarker_COUNT; ++m) {
            ImGui::PushID(m);
            ImPlot::SetNextMarkerStyle(m, mk_size, IMPLOT_AUTO_COL, mk_weight);
            ImPlot::PlotLine("##Filled", xs, ys, 2);
            ImGui::PopID();
            ys[0]--; ys[1]--;
        }
        xs[0] = 6; xs[1] = 9; ys[0] = 10; ys[1] = 11;
        // open markers
        for (int m = 0; m < ImPlotMarker_COUNT; ++m) {
            ImGui::PushID(m);
            ImPlot::SetNextMarkerStyle(m, mk_size, ImVec4(0,0,0,0), mk_weight);
            ImPlot::PlotLine("##Open", xs, ys, 2);
            ImGui::PopID();
            ys[0]--; ys[1]--;
        }

        ImPlot::PlotText("Filled Markers", 2.5f, 6.0f);
        ImPlot::PlotText("Open Markers",   7.5f, 6.0f);

        ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1,0,1,1));
        ImPlot::PlotText("Vertical Text", 5.0f, 6.0f, true);
        ImPlot::PopStyleColor();

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

void ShowDemo_MultipleAxes() {
    static float xs[1001], xs2[1001], ys1[1001], ys2[1001], ys3[1001];
    for (int i = 0; i < 1001; ++i) {
        xs[i]  = (i*0.1f);
        xs2[i] = xs[i] + 10.0f;
        ys1[i] = sinf(xs[i]) * 3 + 1;
        ys2[i] = cosf(xs[i]) * 0.2f + 0.5f;
        ys3[i] = sinf(xs[i]+0.5f) * 100 + 200;
    }

    static bool x2_axis = true;
    static bool y2_axis = true;
    static bool y3_axis = true;

    ImGui::Checkbox("X-Axis 2", &x2_axis);
    ImGui::SameLine();
    ImGui::Checkbox("Y-Axis 2", &y2_axis);
    ImGui::SameLine();
    ImGui::Checkbox("Y-Axis 3", &y3_axis);

    ImGui::BulletText("You can drag axes to the opposite side of the plot.");
    ImGui::BulletText("Hover over legend items to see which axis they are plotted on.");

    if (ImPlot::BeginPlot("Multi-Axis Plot", ImVec2(-1,0))) {
        ImPlot::SetupAxes("X-Axis 1", "Y-Axis 1");
        ImPlot::SetupAxesLimits(0, 100, 0, 10);
        if (x2_axis) {
            ImPlot::SetupAxis(ImAxis_X2, "X-Axis 2",ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_X2, 0, 100);
        }
        if (y2_axis) {
            ImPlot::SetupAxis(ImAxis_Y2, "Y-Axis 2",ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_Y2, 0, 1);
        }
        if (y3_axis) {
            ImPlot::SetupAxis(ImAxis_Y3, "Y-Axis 3",ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_Y3, 0, 300);
        }

        ImPlot::PlotLine("f(x) = x", xs, xs, 1001);
        if (x2_axis) {
            ImPlot::SetAxes(ImAxis_X2, ImAxis_Y1);
            ImPlot::PlotLine("f(x) = sin(x)*3+1", xs2, ys1, 1001);
        }
        if (y2_axis) {
            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
            ImPlot::PlotLine("f(x) = cos(x)*.2+.5", xs, ys2, 1001);
        }
        if (y3_axis) {
            ImPlot::SetAxes(ImAxis_X2, ImAxis_Y3);
            ImPlot::PlotLine("f(x) = sin(x+.5)*100+200 ", xs2, ys3, 1001);
        }
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

void ShowDemo_AutoFittingData() {
    ImGui::BulletText("The Y-axis has been configured to auto-fit to only the data visible in X-axis range.");
    ImGui::BulletText("Zoom and pan the X-axis. Disable Stems to see a difference in fit.");
    ImGui::BulletText("If ImPlotAxisFlags_RangeFit is disabled, the axis will fit ALL data.");

    static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
    static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;

    ImGui::TextUnformatted("X: "); ImGui::SameLine();
    ImGui::CheckboxFlags("ImPlotAxisFlags_AutoFit##X", (unsigned int*)&xflags, ImPlotAxisFlags_AutoFit); ImGui::SameLine();
    ImGui::CheckboxFlags("ImPlotAxisFlags_RangeFit##X", (unsigned int*)&xflags, ImPlotAxisFlags_RangeFit);

    ImGui::TextUnformatted("Y: "); ImGui::SameLine();
    ImGui::CheckboxFlags("ImPlotAxisFlags_AutoFit##Y", (unsigned int*)&yflags, ImPlotAxisFlags_AutoFit); ImGui::SameLine();
    ImGui::CheckboxFlags("ImPlotAxisFlags_RangeFit##Y", (unsigned int*)&yflags, ImPlotAxisFlags_RangeFit);

    static double data[101];
    srand(0);
    for (int i = 0; i < 101; ++i)
        data[i] = 1 + sin(i/10.0f);

    if (ImPlot::BeginPlot("##DataFitting")) {
        ImPlot::SetupAxes("X","Y",xflags,yflags);
        ImPlot::PlotLine("Line",data,101);
        ImPlot::PlotStems("Stems",data,101);
        ImPlot::EndPlot();
    };
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

void ShowDemo_SubplotAxisLinking() {
    static ImPlotSubplotFlags flags = ImPlotSubplotFlags_LinkRows | ImPlotSubplotFlags_LinkCols;
    ImGui::CheckboxFlags("ImPlotSubplotFlags_LinkRows", (unsigned int*)&flags, ImPlotSubplotFlags_LinkRows);
    ImGui::CheckboxFlags("ImPlotSubplotFlags_LinkCols", (unsigned int*)&flags, ImPlotSubplotFlags_LinkCols);
    ImGui::CheckboxFlags("ImPlotSubplotFlags_LinkAllX", (unsigned int*)&flags, ImPlotSubplotFlags_LinkAllX);
    ImGui::CheckboxFlags("ImPlotSubplotFlags_LinkAllY", (unsigned int*)&flags, ImPlotSubplotFlags_LinkAllY);

    static int rows = 2;
    static int cols = 2;
    if (ImPlot::BeginSubplots("##AxisLinking", rows, cols, ImVec2(-1,400), flags)) {
        for (int i = 0; i < rows*cols; ++i) {
            if (ImPlot::BeginPlot("")) {
                ImPlot::SetupAxesLimits(0,1000,-1,1);
                float fc = 0.01f;
                ImPlot::PlotLineG("common",SinewaveGetter,&fc,1000);
                ImPlot::EndPlot();
            }
        }
        ImPlot::EndSubplots();
    }
}

void ShowDemo_DragPoints() {
    ImGui::BulletText("Click and drag each point.");
    static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
    ImGui::CheckboxFlags("NoCursors", (unsigned int*)&flags, ImPlotDragToolFlags_NoCursors); ImGui::SameLine();
    ImGui::CheckboxFlags("NoFit", (unsigned int*)&flags, ImPlotDragToolFlags_NoFit); ImGui::SameLine();
    ImGui::CheckboxFlags("NoInput", (unsigned int*)&flags, ImPlotDragToolFlags_NoInputs);
    ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
    if (ImPlot::BeginPlot("##Bezier",ImVec2(-1,0),ImPlotFlags_CanvasOnly)) {
        ImPlot::SetupAxes(0,0,ax_flags,ax_flags);
        ImPlot::SetupAxesLimits(0,1,0,1);
        static ImPlotPoint P[] = {ImPlotPoint(.05f,.05f), ImPlotPoint(0.2,0.4),  ImPlotPoint(0.8,0.6),  ImPlotPoint(.95f,.95f)};

        ImPlot::DragPoint(0,&P[0].x,&P[0].y, ImVec4(0,0.9f,0,1),4,flags);
        ImPlot::DragPoint(1,&P[1].x,&P[1].y, ImVec4(1,0.5f,1,1),4,flags);
        ImPlot::DragPoint(2,&P[2].x,&P[2].y, ImVec4(0,0.5f,1,1),4,flags);
        ImPlot::DragPoint(3,&P[3].x,&P[3].y, ImVec4(0,0.9f,0,1),4,flags);

        static ImPlotPoint B[100];
        for (int i = 0; i < 100; ++i) {
            double t  = i / 99.0;
            double u  = 1 - t;
            double w1 = u*u*u;
            double w2 = 3*u*u*t;
            double w3 = 3*u*t*t;
            double w4 = t*t*t;
            B[i] = ImPlotPoint(w1*P[0].x + w2*P[1].x + w3*P[2].x + w4*P[3].x, w1*P[0].y + w2*P[1].y + w3*P[2].y + w4*P[3].y);
        }

        ImPlot::SetNextLineStyle(ImVec4(1,0.5f,1,1));
        ImPlot::PlotLine("##h1",&P[0].x, &P[0].y, 2, 0, sizeof(ImPlotPoint));
        ImPlot::SetNextLineStyle(ImVec4(0,0.5f,1,1));
        ImPlot::PlotLine("##h2",&P[2].x, &P[2].y, 2, 0, sizeof(ImPlotPoint));
        ImPlot::SetNextLineStyle(ImVec4(0,0.9f,0,1), 2);
        ImPlot::PlotLine("##bez",&B[0].x, &B[0].y, 100, 0, sizeof(ImPlotPoint));

        ImPlot::EndPlot();
    }
}

void ShowDemo_DragLines() {
    ImGui::BulletText("Click and drag the horizontal and vertical lines.");
    static double x1 = 0.2;
    static double x2 = 0.8;
    static double y1 = 0.25;
    static double y2 = 0.75;
    static double f = 0.1;
    static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
    ImGui::CheckboxFlags("NoCursors", (unsigned int*)&flags, ImPlotDragToolFlags_NoCursors); ImGui::SameLine();
    ImGui::CheckboxFlags("NoFit", (unsigned int*)&flags, ImPlotDragToolFlags_NoFit); ImGui::SameLine();
    ImGui::CheckboxFlags("NoInput", (unsigned int*)&flags, ImPlotDragToolFlags_NoInputs);
    if (ImPlot::BeginPlot("##lines",ImVec2(-1,0))) {
        ImPlot::SetupAxesLimits(0,1,0,1);
        ImPlot::DragLineX(0,&x1,ImVec4(1,1,1,1),1,flags);
        ImPlot::DragLineX(1,&x2,ImVec4(1,1,1,1),1,flags);
        ImPlot::DragLineY(2,&y1,ImVec4(1,1,1,1),1,flags);
        ImPlot::DragLineY(3,&y2,ImVec4(1,1,1,1),1,flags);
        double xs[1000], ys[1000];
        for (int i = 0; i < 1000; ++i) {
            xs[i] = (x2+x1)/2+fabs(x2-x1)*(i/1000.0f - 0.5f);
            ys[i] = (y1+y2)/2+fabs(y2-y1)/2*sin(f*i/10);
        }
        ImPlot::PlotLine("Interactive Data", xs, ys, 1000);
        ImPlot::DragLineY(120482,&f,ImVec4(1,0.5f,1,1),1,flags);
        ImPlot::EndPlot();
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
	//データ出力
	if (ImGui::Button("DataOutPut"))
	{

	}
    if (ImGui::Button("Clear Queries"))
        rects.shrink(0);

	static ScrollingBuffer sdata1, sdata2;
	static RollingBuffer   rdata1, rdata2;
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotPoint pt;
	ImVec2 mouse = ImGui::GetMousePos();
	static bool s_bPlay = false;
	static bool s_bLoop = false;
	static bool s_bTag = false;
	static int s_nIndex = 0;
	static int s_nSelectPt = 0;
	static float flt = 0.0f;
	static float s_fStartTimeA = rdata1.Span;
	static float s_fStartTimeB = rdata2.Span;
	static float s_fStopTime = 0.0f;
	static float s_History = 5.0f;
	static float t = 0;
	static double s_dMin = 0.0f;
	static double s_dMax = 10.0f;

	/*data[i].x(y)で数値がとれます。double型なので、floatでやるとバグる可能性あります*/
	ImGui::Text("Ctrl + Left Click : Set point");
	ImGui::Text("Del + Left Click : Delete point");
	ImGui::Text("Double Left Click : Auto Fit");

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
		s_dMin = 0.0f;
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

    if (ImPlot::BeginPlot("Timeline")) 
	{
		ImPlot::SetupAxes(0, 0, flags, flags);
        ImPlot::SetupAxesLimits(0,1,0,1);

		//グラフの再生
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

					s_nSelectPt = i;
					ptNum++;
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

void ShowDemo_Tables() {
#ifdef IMGUI_HAS_TABLE
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                   ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
    static bool anim = true;
    static int offset = 0;
    ImGui::BulletText("Plots can be used inside of ImGui tables as another means of creating subplots.");
    ImGui::Checkbox("Animate",&anim);
    if (anim)
        offset = (offset + 1) % 100;
    if (ImGui::BeginTable("##table", 3, flags, ImVec2(-1,0))) {
        ImGui::TableSetupColumn("Electrode", ImGuiTableColumnFlags_WidthFixed, 75.0f);
        ImGui::TableSetupColumn("Voltage", ImGuiTableColumnFlags_WidthFixed, 75.0f);
        ImGui::TableSetupColumn("EMG Signal");
        ImGui::TableHeadersRow();
        ImPlot::PushColormap(ImPlotColormap_Cool);
        for (int row = 0; row < 10; row++) {
            ImGui::TableNextRow();
            static float data[100];
            srand(row);
            for (int i = 0; i < 100; ++i)
                data[i] = RandomRange(0.0f,10.0f);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("EMG %d", row);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.3f V", data[offset]);
            ImGui::TableSetColumnIndex(2);
            ImGui::PushID(row);
            MyImPlot::Sparkline("##spark",data,100,0,11.0f,offset,ImPlot::GetColormapColor(row),ImVec2(-1, 35));
            ImGui::PopID();
        }
        ImPlot::PopColormap();
        ImGui::EndTable();
    }
#else
    ImGui::BulletText("You need to merge the ImGui 'tables' branch for this section.");
#endif
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

void ShowDemo_TickLabels()  {
    static bool custom_fmt    = true;
    static bool custom_ticks  = false;
    static bool custom_labels = true;
    ImGui::Checkbox("Show Custom Format", &custom_fmt);
    ImGui::SameLine();
    ImGui::Checkbox("Show Custom Ticks", &custom_ticks);
    if (custom_ticks) {
        ImGui::SameLine();
        ImGui::Checkbox("Show Custom Labels", &custom_labels);
    }
    const double pi = 3.14;
    const char* pi_str[] = {"PI"};
    static double yticks[] = {100,300,700,900};
    static const char*  ylabels[] = {"One","Three","Seven","Nine"};
    static double yticks_aux[] = {0.2,0.4,0.6};
    static const char* ylabels_aux[] = {"A","B","C","D","E","F"};

    if (ImPlot::BeginPlot("##Ticks")) {
        ImPlot::SetupAxesLimits(2.5,5,0,1000);
        ImPlot::SetupAxis(ImAxis_Y2, NULL, ImPlotAxisFlags_AuxDefault);
        ImPlot::SetupAxis(ImAxis_Y3, NULL, ImPlotAxisFlags_AuxDefault);
        if (custom_fmt) {
            ImPlot::SetupAxisFormat(ImAxis_X1, "%g ms");
            ImPlot::SetupAxisFormat(ImAxis_Y1, MetricFormatter, (void*)"Hz");
            ImPlot::SetupAxisFormat(ImAxis_Y2, "%g dB");
            ImPlot::SetupAxisFormat(ImAxis_Y3, MetricFormatter, (void*)"m");
        }
        if (custom_ticks) {
            ImPlot::SetupAxisTicks(ImAxis_X1, &pi,1,custom_labels ? pi_str : NULL, true);
            ImPlot::SetupAxisTicks(ImAxis_Y1, yticks, 4, custom_labels ? ylabels : NULL, false);
            ImPlot::SetupAxisTicks(ImAxis_Y2, yticks_aux, 3, custom_labels ? ylabels_aux : NULL, false);
            ImPlot::SetupAxisTicks(ImAxis_Y3, 0, 1, 6, custom_labels ? ylabels_aux : NULL, false);
        }
        ImPlot::EndPlot();
    }
}

//-----------------------------------------------------------------------------
// DEMO WINDOW
//-----------------------------------------------------------------------------

void ShowDemoWindow(bool* p_open) 
{
    static bool show_imgui_metrics       = false;
    static bool show_implot_metrics      = false;
    static bool show_imgui_style_editor  = false;
    static bool show_implot_style_editor = false;
    static bool show_implot_benchmark    = false;
    if (show_imgui_metrics)
	{
        ImGui::ShowMetricsWindow(&show_imgui_metrics);
    }
    if (show_implot_metrics) 
	{
        ImPlot::ShowMetricsWindow(&show_implot_metrics);
    }
    if (show_imgui_style_editor) 
	{
        ImGui::Begin("Style Editor (ImGui)", &show_imgui_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
    if (show_implot_style_editor) 
	{
        ImGui::SetNextWindowSize(ImVec2(415,762), ImGuiCond_Appearing);
        ImGui::Begin("Style Editor (ImPlot)", &show_implot_style_editor);
        ImPlot::ShowStyleEditor();
        ImGui::End();
    }
    if (show_implot_benchmark) 
	{
        ImGui::SetNextWindowSize(ImVec2(530,740), ImGuiCond_Appearing);
        ImGui::Begin("ImPlot Benchmark Tool", &show_implot_benchmark);
        ImPlot::ShowBenchmarkTool();
        ImGui::End();
        return;
    }
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
    ImGui::Begin("ImPlot Demo", p_open, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
	{
        if (ImGui::BeginMenu("Tools")) 
		{
            ImGui::MenuItem("Metrics (ImGui)",       NULL, &show_imgui_metrics);
            ImGui::MenuItem("Metrics (ImPlot)",      NULL, &show_implot_metrics);
            ImGui::MenuItem("Style Editor (ImGui)",  NULL, &show_imgui_style_editor);
            ImGui::MenuItem("Style Editor (ImPlot)", NULL, &show_implot_style_editor);
            ImGui::MenuItem("Benchmark",             NULL, &show_implot_benchmark);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
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
		   /*
            if (ImGui::CollapsingHeader("Line Plots"))
                ShowDemo_LinePlots();*/

       /*     if (ImGui::CollapsingHeader("Realtime Plots"))
                ShowDemo_RealtimePlots();*/

			
            /*if (ImGui::CollapsingHeader("Stairstep Plots"))
                ShowDemo_StairstepPlots();
            if (ImGui::CollapsingHeader("Bar Groups"))
                ShowDemo_BarGroups();
            if (ImGui::CollapsingHeader("Error Bars"))
                ShowDemo_ErrorBars();
            if (ImGui::CollapsingHeader("Stem Plots##"))
                ShowDemo_StemPlots();
            if (ImGui::CollapsingHeader("Infinite Lines"))
                ShowDemo_InfiniteLines();
            if (ImGui::CollapsingHeader("Histogram"))
                ShowDemo_Histogram();
            if (ImGui::CollapsingHeader("Digital Plots"))
                ShowDemo_DigitalPlots();*/
				

          /*  if (ImGui::CollapsingHeader("Markers and Text"))
                ShowDemo_MarkersAndText();
            ImGui::EndTabItem();
        }*/

      /*  if (ImGui::BeginTabItem("Subplots")) 
	  {
            if (ImGui::CollapsingHeader("Sizing"))
                ShowDemo_SubplotsSizing();
            if (ImGui::CollapsingHeader("Item Sharing"))
                ShowDemo_SubplotItemSharing();
            if (ImGui::CollapsingHeader("Axis Linking"))
                ShowDemo_SubplotAxisLinking();
            if (ImGui::CollapsingHeader("Tables"))
                ShowDemo_Tables();
            ImGui::EndTabItem();
        }*/

       /* if (ImGui::BeginTabItem("Axes")) 
		{
            if (ImGui::CollapsingHeader("Log Axes"))
                ShowDemo_LogAxes();
            if (ImGui::CollapsingHeader("Multiple Axes"))
                ShowDemo_MultipleAxes();
            if (ImGui::CollapsingHeader("Tick Labels"))
                ShowDemo_TickLabels();
            if (ImGui::CollapsingHeader("Linked Axes"))
                ShowDemo_LinkedAxes();
            if (ImGui::CollapsingHeader("Equal Axes"))
                ShowDemo_EqualAxes();
            if (ImGui::CollapsingHeader("Auto-Fitting Data"))
                ShowDemo_AutoFittingData();
            ImGui::EndTabItem();
        }*/
        if (ImGui::BeginTabItem("Tools")) 
		{
      /*      if (ImGui::CollapsingHeader("Offset and Stride"))
                ShowDemo_OffsetAndStride();
            if (ImGui::CollapsingHeader("Drag Points"))
                ShowDemo_DragPoints();
            if (ImGui::CollapsingHeader("Drag Lines"))
                ShowDemo_DragLines();*/

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

void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size) {
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0,0));
    if (ImPlot::BeginPlot(id,size,ImPlotFlags_CanvasOnly|ImPlotFlags_NoChild)) {
        ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxesLimits(0, count - 1, min_v, max_v, ImGuiCond_Always);
        ImPlot::PushStyleColor(ImPlotCol_Line, col);
        ImPlot::PlotLine(id, values, count, 1, 0, offset);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotShaded(id, values, count, 0, 1, 0, offset);
        ImPlot::PopStyleVar();
        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }
    ImPlot::PopStyleVar();
}

void StyleSeaborn() {

    ImPlotStyle& style              = ImPlot::GetStyle();

    ImVec4* colors                  = style.Colors;
    colors[ImPlotCol_Line]          = IMPLOT_AUTO_COL;
    colors[ImPlotCol_Fill]          = IMPLOT_AUTO_COL;
    colors[ImPlotCol_MarkerOutline] = IMPLOT_AUTO_COL;
    colors[ImPlotCol_MarkerFill]    = IMPLOT_AUTO_COL;
    colors[ImPlotCol_ErrorBar]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImPlotCol_FrameBg]       = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImPlotCol_PlotBg]        = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    colors[ImPlotCol_PlotBorder]    = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImPlotCol_LegendBg]      = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    colors[ImPlotCol_LegendBorder]  = ImVec4(0.80f, 0.81f, 0.85f, 1.00f);
    colors[ImPlotCol_LegendText]    = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImPlotCol_TitleText]     = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImPlotCol_InlayText]     = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImPlotCol_AxisText]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImPlotCol_AxisGrid]      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImPlotCol_AxisBgHovered]   = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    colors[ImPlotCol_AxisBgActive]    = ImVec4(0.92f, 0.92f, 0.95f, 0.75f);
    colors[ImPlotCol_Selection]     = ImVec4(1.00f, 0.65f, 0.00f, 1.00f);
    colors[ImPlotCol_Crosshairs]    = ImVec4(0.23f, 0.10f, 0.64f, 0.50f);

    style.LineWeight       = 1.5;
    style.Marker           = ImPlotMarker_None;
    style.MarkerSize       = 4;
    style.MarkerWeight     = 1;
    style.FillAlpha        = 1.0f;
    style.ErrorBarSize     = 5;
    style.ErrorBarWeight   = 1.5f;
    style.DigitalBitHeight = 8;
    style.DigitalBitGap    = 4;
    style.PlotBorderSize   = 0;
    style.MinorAlpha       = 1.0f;
    style.MajorTickLen     = ImVec2(0,0);
    style.MinorTickLen     = ImVec2(0,0);
    style.MajorTickSize    = ImVec2(0,0);
    style.MinorTickSize    = ImVec2(0,0);
    style.MajorGridSize    = ImVec2(1.2f,1.2f);
    style.MinorGridSize    = ImVec2(1.2f,1.2f);
    style.PlotPadding      = ImVec2(12,12);
    style.LabelPadding     = ImVec2(5,5);
    style.LegendPadding    = ImVec2(5,5);
    style.MousePosPadding  = ImVec2(5,5);
    style.PlotMinSize      = ImVec2(300,225);
}

} // namespaece MyImPlot

// WARNING:
//
// You can use "implot_internal.h" to build custom plotting fuctions or extend ImPlot.
// However, note that forward compatibility of this file is not guaranteed and the
// internal API is subject to change. At some point we hope to bring more of this
// into the public API and expose the necessary building blocks to fully support
// custom plotters. For now, proceed at your own risk!

#include "implot_internal.h"

namespace MyImPlot {

template <typename T>
int BinarySearch(const T* arr, int l, int r, T x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return BinarySearch(arr, l, mid - 1, x);
        return BinarySearch(arr, mid + 1, r, x);
    }
    return -1;
}

void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip, float width_percent, ImVec4 bullCol, ImVec4 bearCol) {

    // get ImGui window DrawList
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    // calc real value width
    double half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;

    // custom tool
    if (ImPlot::IsPlotHovered() && tooltip) {
        ImPlotPoint mouse   = ImPlot::GetPlotMousePos();
        mouse.x             = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), ImPlotTimeUnit_Day).ToDouble();
        float  tool_l       = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
        float  tool_r       = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
        float  tool_t       = ImPlot::GetPlotPos().y;
        float  tool_b       = tool_t + ImPlot::GetPlotSize().y;
        ImPlot::PushPlotClipRect();
        draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128,128,128,64));
        ImPlot::PopPlotClipRect();
        // find mouse location index
        int idx = BinarySearch(xs, 0, count - 1, mouse.x);
        // render tool tip (won't be affected by plot clip rect)
        if (idx != -1) {
            ImGui::BeginTooltip();
            char buff[32];
            ImPlot::FormatDate(ImPlotTime::FromDouble(xs[idx]),buff,32,ImPlotDateFmt_DayMoYr,ImPlot::GetStyle().UseISO8601);
            ImGui::Text("Day:   %s",  buff);
            ImGui::Text("Open:  $%.2f", opens[idx]);
            ImGui::Text("Close: $%.2f", closes[idx]);
            ImGui::Text("Low:   $%.2f", lows[idx]);
            ImGui::Text("High:  $%.2f", highs[idx]);
            ImGui::EndTooltip();
        }
    }

    // begin plot item
    if (ImPlot::BeginItem(label_id)) {
        // override legend icon color
        ImPlot::GetCurrentItem()->Color = IM_COL32(64,64,64,255);
        // fit data if requested
        if (ImPlot::FitThisFrame()) {
            for (int i = 0; i < count; ++i) {
                ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
                ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
            }
        }
        // render data
        for (int i = 0; i < count; ++i) {
            ImVec2 open_pos  = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
            ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
            ImVec2 low_pos   = ImPlot::PlotToPixels(xs[i], lows[i]);
            ImVec2 high_pos  = ImPlot::PlotToPixels(xs[i], highs[i]);
            ImU32 color      = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
            draw_list->AddLine(low_pos, high_pos, color);
            draw_list->AddRectFilled(open_pos, close_pos, color);
        }

        // end plot item
        ImPlot::EndItem();
    }
}

} // namespace MyImplot

namespace ImPlot {

//-----------------------------------------------------------------------------
// BENCHMARK
//-----------------------------------------------------------------------------

struct BenchData {
    BenchData() {
        float y = RandomRange(0.0f,1.0f);
        Data = new float[1000];
        for (int i = 0; i < 1000; ++i) {
            Data[i] = y + RandomRange(-0.01f,0.01f);
        }
        Col = ImVec4(RandomRange(0.0f,1.0f),RandomRange(0.0f,1.0f),RandomRange(0.0f,1.0f),0.5f);
    }
    ~BenchData() { delete[] Data; }
    float* Data;
    ImVec4 Col;
};

enum BenchMode {
    Line = 0,
    LineG = 1,
    Shaded = 2,
    Scatter = 3,
    Bars = 4
};

struct BenchRecord {
    int Mode;
    bool AA;
    ImVector<ImPlotPoint> Data;
};

ImPlotPoint BenchmarkGetter(void* data, int idx) {
    float* values = (float*)data;
    return ImPlotPoint(idx, values[idx]);
}

void ShowBenchmarkTool() {
    static const int max_items = 500;
    static BenchData items[max_items];
    static bool running = false;
    static int frames   = 60;
    static int L        = 0;
    static int F        = 0;
    static double t1, t2;
    static int mode     = BenchMode::Line;
    const char* names[] = {"Line","LineG","Shaded","Scatter","Bars"};

    static ImVector<BenchRecord> records;

    if (running) {
        F++;
        if (F == frames) {
            t2 = ImGui::GetTime();
            records.back().Data.push_back(ImPlotPoint(L, frames / (t2 - t1)));
            L  += 5;
            F  = 0;
            t1 = ImGui::GetTime();
        }
        if (L > max_items) {
            running = false;
            L = max_items;
        }
    }

    ImGui::Text("ImDrawIdx: %d-bit", (int)(sizeof(ImDrawIdx) * 8));
    ImGui::Text("ImGuiBackendFlags_RendererHasVtxOffset: %s", (ImGui::GetIO().BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) ? "True" : "False");
    ImGui::Text("%.2f FPS", ImGui::GetIO().Framerate);

    ImGui::Separator();

    bool was_running = running;
    if (was_running) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
    }
    if (ImGui::Button("Benchmark")) {
        running = true;
        L = F = 0;
        records.push_back(BenchRecord());
        records.back().Data.reserve(max_items+1);
        records.back().Mode = mode;
        records.back().AA   = ImPlot::GetStyle().AntiAliasedLines;
        t1 = ImGui::GetTime();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::Combo("##Mode",&mode,names,4);
    ImGui::SameLine();

    ImGui::Checkbox("Anti-Aliased Lines", &ImPlot::GetStyle().AntiAliasedLines);
    if (was_running) { ImGui::PopItemFlag(); ImGui::PopStyleVar(); }

    ImGui::ProgressBar((float)L / (float)(max_items - 1));

    if (ImPlot::BeginPlot("##Bench",ImVec2(-1,0),ImPlotFlags_NoChild | ImPlotFlags_CanvasOnly)) {
        ImPlot::SetupAxes(NULL,NULL,ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxesLimits(0,1000,0,1,ImGuiCond_Always);
        if (running) {
            if (mode == BenchMode::Line) {
                for (int i = 0; i < L; ++i) {
                    ImGui::PushID(i);
                    ImPlot::SetNextLineStyle(items[i].Col);
                    ImPlot::PlotLine("##item", items[i].Data, 1000);
                    ImGui::PopID();
                }
            }
            else if (mode == BenchMode::LineG) {
                for (int i = 0; i < L; ++i) {
                    ImGui::PushID(i);
                    ImPlot::SetNextLineStyle(items[i].Col);
                    ImPlot::PlotLineG("##item",BenchmarkGetter,items[i].Data,1000);
                    ImGui::PopID();
                }
            }
            else if (mode == BenchMode::Shaded) {
                for (int i = 0; i < L; ++i) {
                    ImGui::PushID(i);
                    ImPlot::SetNextFillStyle(items[i].Col,0.5f);
                    ImPlot::PlotShaded("##item", items[i].Data, 1000);
                    ImGui::PopID();
                }
            }
            else if (mode == BenchMode::Scatter) {
                for (int i = 0; i < L; ++i) {
                    ImGui::PushID(i);
                    ImPlot::SetNextLineStyle(items[i].Col);
                    ImPlot::PlotScatter("##item", items[i].Data, 1000);
                    ImGui::PopID();
                }
            }
            else if (mode == BenchMode::Bars) {
                for (int i = 0; i < L; ++i) {
                    ImGui::PushID(i);
                    ImPlot::SetNextFillStyle(items[i].Col,0.5f);
                    ImPlot::PlotBars("##item", items[i].Data, 1000);
                    ImGui::PopID();
                }
            }
        }
        ImPlot::EndPlot();
    }
    static char buffer[64];
    if (ImPlot::BeginPlot("##Stats", ImVec2(-1,0), ImPlotFlags_NoChild)) {
        ImPlot::SetupAxes("Items (1,000 pts each)", "Framerate (Hz)");
        ImPlot::SetupAxesLimits(0,500,0,500,ImGuiCond_Always);
        for (int run = 0; run < records.size(); ++run) {
            if (records[run].Data.Size > 1) {
                sprintf(buffer, "B%d-%s%s", run + 1, names[records[run].Mode], records[run].AA ? "-AA" : "");
                ImVector<ImPlotPoint>& d = records[run].Data;
                ImPlot::PlotLine(buffer, &d[0].x, &d[0].y, d.Size, 0, 2*sizeof(double));
            }
        }
        ImPlot::EndPlot();
    }
}

}