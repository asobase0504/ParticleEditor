// ImPlot v0.13 WIP

// We define this so that the demo does not accidentally use deprecated API
#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

#include "implot.h"
#include "imgui.h"
#include <fstream>
#include "nlohmann/json.hpp"

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

	// Example for Tables section.
	void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size);

	// Example for Custom Plotters and Tooltips section.
	void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0, 1, 0, 1), ImVec4 bearCol = ImVec4(1, 0, 0, 1));

	// Example for Custom Styles section.
	void StyleSeaborn();

} // namespace MyImPlot

namespace ImPlot {

void ShowBenchmarkTool();

template <typename T>
inline T RandomRange(T min, T max) {
	T scale = rand() / (T)RAND_MAX;
	return min + scale * (max - min);
}

ImVec4 RandomColor() {
	ImVec4 col;
	col.x = RandomRange(0.0f, 1.0f);
	col.y = RandomRange(0.0f, 1.0f);
	col.z = RandomRange(0.0f, 1.0f);
	col.w = 1.0f;
	return col;
}

double RandomGauss() {
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
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
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
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
		return 0.5 + 0.25 * sin(t / 86400 / 12) + 0.005 * sin(t / 3600);
	}
	double* Ts;
	double* Ys;
	static const int Size = 60 * 60 * 24 * 366;
};

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
		ImPlot::SetupAxes("x", "f(x)");
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
		ImPlot::SetupAxes("x", "f(x)");
		ImPlot::PlotStairs("Signal 1", ys1, 101, 0.01f);
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 2.0f);
		ImPlot::PlotStairs("Signal 2", ys2, 101, 0.01f);
		ImPlot::EndPlot();
	}
}

ImPlotPoint SinewaveGetter(void* data, int i) {
	float f = *(float*)data;
	return ImPlotPoint(i, sinf(f*i));
}

void ShowDemo_SubplotsSizing() {

	static ImPlotSubplotFlags flags = ImPlotSubplotFlags_None;
	ImGui::CheckboxFlags("ImPlotSubplotFlags_NoResize", (unsigned int*)&flags, ImPlotSubplotFlags_NoResize);
	ImGui::CheckboxFlags("ImPlotSubplotFlags_NoTitle", (unsigned int*)&flags, ImPlotSubplotFlags_NoTitle);

	static int rows = 3;
	static int cols = 3;
	ImGui::SliderInt("Rows", &rows, 1, 5);
	ImGui::SliderInt("Cols", &cols, 1, 5);
	static float rratios[] = { 5,1,1,1,1,1 };
	static float cratios[] = { 5,1,1,1,1,1 };
	ImGui::DragScalarN("Row Ratios", ImGuiDataType_Float, rratios, rows, 0.01f, 0);
	ImGui::DragScalarN("Col Ratios", ImGuiDataType_Float, cratios, cols, 0.01f, 0);
	if (ImPlot::BeginSubplots("My Subplots", rows, cols, ImVec2(-1, 400), flags, rratios, cratios)) {
		for (int i = 0; i < rows*cols; ++i) {
			if (ImPlot::BeginPlot("", ImVec2(), ImPlotFlags_NoLegend)) {
				ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
				float fi = 0.01f * (i + 1);
				ImPlot::SetNextLineStyle(SampleColormap((float)i / (float)(rows*cols - 1), ImPlotColormap_Jet));
				ImPlot::PlotLineG("data", SinewaveGetter, &fi, 1000);
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
	static int id[] = { 0,1,2,3,4,5 };
	static int curj = -1;
	if (ImPlot::BeginSubplots("##ItemSharing", rows, cols, ImVec2(-1, 400), flags)) {
		for (int i = 0; i < rows*cols; ++i) {
			if (ImPlot::BeginPlot("")) {
				float fc = 0.01f;
				ImPlot::PlotLineG("common", SinewaveGetter, &fc, 1000);
				for (int j = 0; j < 6; ++j) {
					if (id[j] == i) {
						char label[8];
						float fj = 0.01f * (j + 2);
						sprintf(label, "data%d", j);
						ImPlot::PlotLineG(label, SinewaveGetter, &fj, 1000);
						if (ImPlot::BeginDragDropSourceItem(label)) {
							curj = j;
							ImGui::SetDragDropPayload("MY_DND", NULL, 0);
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
			data.push_back(ImPlotPoint(x, y));
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

	//現在選択している点の詳細
	ImGui::Text("Selected Point : %d", s_nSelectPt);

	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		if (s_nSelectPt > 0)
		{
			s_nSelectPt--;
		}
	}

	ImGui::SameLine();
	if (ImGui::ArrowButton("##Right", ImGuiDir_Right))
	{
		if (s_nSelectPt < s_nIndex)
		{
			s_nSelectPt++;
		}
	}

	ImGui::DragScalar("Select Key", ImGuiDataType_Double, &data[s_nSelectPt].x, 0.01f);
	ImGui::DragScalar("Select Value", ImGuiDataType_Double, &data[s_nSelectPt].y, 0.01f);

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
		ImPlot::SetupAxes(0, 0, flags, flags);
		ImPlot::SetupAxesLimits(0, 1, 0, 1);
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
			ImPlot::DragPoint(s_nIndex + i, &data[i].x, &data[i].y, ImVec4(0, 0.9f, 0, 1), 4, flags);
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
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6);

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
			ImPlotPoint centroid = FindCentroid(data, rects[i], cnt);

			if (cnt > 0) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6);
				ImPlot::PlotScatter("Centroid", &centroid.x, &centroid.y, 1);
			}

			ImPlot::DragRect(i, &rects[i].X.Min, &rects[i].Y.Min, &rects[i].X.Max, &rects[i].Y.Max, ImVec4(1, 0, 1, 1));
		}

		limits = ImPlot::GetPlotLimits();
		ImPlot::EndPlot();
	}
}

void MetricFormatter(double value, char* buff, int size, void* data) {
	const char* unit = (const char*)data;
	static double v[] = { 1000000000,1000000,1000,1,0.001,0.000001,0.000000001 };
	static const char* p[] = { "G","M","k","","m","u","n" };
	if (value == 0) {
		snprintf(buff, size, "0 %s", unit);
		return;
	}
	for (int i = 0; i < 7; ++i) {
		if (fabs(value) >= v[i]) {
			snprintf(buff, size, "%g %s%s", value / v[i], p[i], unit);
			return;
		}
	}
	snprintf(buff, size, "%g %s%s", value / v[6], p[6], unit);
}

//-----------------------------------------------------------------------------
// DEMO WINDOW
//-----------------------------------------------------------------------------

void ShowDemoWindow(bool* p_open) {
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
	ImGui::Begin("ImPlot Demo", p_open, ImGuiWindowFlags_MenuBar);
	//-------------------------------------------------------------------------
	ImGui::Text("ImPlot says hello. (%s)", IMPLOT_VERSION);
	// display warning about 16-bit indices
	static bool showWarning = sizeof(ImDrawIdx) * 8 == 16 && (ImGui::GetIO().BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) == false;
	if (showWarning) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::TextWrapped("WARNING: ImDrawIdx is 16-bit and ImGuiBackendFlags_RendererHasVtxOffset is false. Expect visual glitches and artifacts! See README for more information.");
		ImGui::PopStyleColor();
	}

	ImGui::Spacing();

	if (ImGui::BeginTabBar("ImPlotDemoTabs"))
	{/*
		if (ImGui::BeginTabItem("Plots")) {

		     if (ImGui::CollapsingHeader("Line Plots"))
		         ShowDemo_LinePlots();
		     if (ImGui::CollapsingHeader("Stairstep Plots"))
		         ShowDemo_StairstepPlots();
		     ImGui::EndTabItem();
		 }

		     if (ImGui::BeginTabItem("Subplots")) 
		     {
		         if (ImGui::CollapsingHeader("Sizing"))
		             ShowDemo_SubplotsSizing();
		         if (ImGui::CollapsingHeader("Item Sharing"))
		             ShowDemo_SubplotItemSharing();
		         ImGui::EndTabItem();
		     }*/

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

	ImPlotPoint SineWave(void* data, int idx) {
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
		return ImPlotPoint(0.5f + (a + b*Th / (2.0f * (float) 3.14))*cos(Th),
			0.5f + (a + b*Th / (2.0f * (float)3.14))*sin(Th));
	}

	void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size) {
		ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
		if (ImPlot::BeginPlot(id, size, ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild)) {
			ImPlot::SetupAxes(0, 0, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
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

		ImPlotStyle& style = ImPlot::GetStyle();

		ImVec4* colors = style.Colors;
		colors[ImPlotCol_Line] = IMPLOT_AUTO_COL;
		colors[ImPlotCol_Fill] = IMPLOT_AUTO_COL;
		colors[ImPlotCol_MarkerOutline] = IMPLOT_AUTO_COL;
		colors[ImPlotCol_MarkerFill] = IMPLOT_AUTO_COL;
		colors[ImPlotCol_ErrorBar] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImPlotCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImPlotCol_PlotBg] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
		colors[ImPlotCol_PlotBorder] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImPlotCol_LegendBg] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
		colors[ImPlotCol_LegendBorder] = ImVec4(0.80f, 0.81f, 0.85f, 1.00f);
		colors[ImPlotCol_LegendText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImPlotCol_TitleText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImPlotCol_InlayText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImPlotCol_AxisText] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImPlotCol_AxisGrid] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImPlotCol_AxisBgHovered] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
		colors[ImPlotCol_AxisBgActive] = ImVec4(0.92f, 0.92f, 0.95f, 0.75f);
		colors[ImPlotCol_Selection] = ImVec4(1.00f, 0.65f, 0.00f, 1.00f);
		colors[ImPlotCol_Crosshairs] = ImVec4(0.23f, 0.10f, 0.64f, 0.50f);

		style.LineWeight = 1.5;
		style.Marker = ImPlotMarker_None;
		style.MarkerSize = 4;
		style.MarkerWeight = 1;
		style.FillAlpha = 1.0f;
		style.ErrorBarSize = 5;
		style.ErrorBarWeight = 1.5f;
		style.DigitalBitHeight = 8;
		style.DigitalBitGap = 4;
		style.PlotBorderSize = 0;
		style.MinorAlpha = 1.0f;
		style.MajorTickLen = ImVec2(0, 0);
		style.MinorTickLen = ImVec2(0, 0);
		style.MajorTickSize = ImVec2(0, 0);
		style.MinorTickSize = ImVec2(0, 0);
		style.MajorGridSize = ImVec2(1.2f, 1.2f);
		style.MinorGridSize = ImVec2(1.2f, 1.2f);
		style.PlotPadding = ImVec2(12, 12);
		style.LabelPadding = ImVec2(5, 5);
		style.LegendPadding = ImVec2(5, 5);
		style.MousePosPadding = ImVec2(5, 5);
		style.PlotMinSize = ImVec2(300, 225);
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
			ImPlotPoint mouse = ImPlot::GetPlotMousePos();
			mouse.x = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), ImPlotTimeUnit_Day).ToDouble();
			float  tool_l = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
			float  tool_r = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
			float  tool_t = ImPlot::GetPlotPos().y;
			float  tool_b = tool_t + ImPlot::GetPlotSize().y;
			ImPlot::PushPlotClipRect();
			draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128, 128, 128, 64));
			ImPlot::PopPlotClipRect();
			// find mouse location index
			int idx = BinarySearch(xs, 0, count - 1, mouse.x);
			// render tool tip (won't be affected by plot clip rect)
			if (idx != -1) {
				ImGui::BeginTooltip();
				char buff[32];
				ImPlot::FormatDate(ImPlotTime::FromDouble(xs[idx]), buff, 32, ImPlotDateFmt_DayMoYr, ImPlot::GetStyle().UseISO8601);
				ImGui::Text("Day:   %s", buff);
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
			ImPlot::GetCurrentItem()->Color = IM_COL32(64, 64, 64, 255);
			// fit data if requested
			if (ImPlot::FitThisFrame()) {
				for (int i = 0; i < count; ++i) {
					ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
					ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
				}
			}
			// render data
			for (int i = 0; i < count; ++i) {
				ImVec2 open_pos = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
				ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
				ImVec2 low_pos = ImPlot::PlotToPixels(xs[i], lows[i]);
				ImVec2 high_pos = ImPlot::PlotToPixels(xs[i], highs[i]);
				ImU32 color = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
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
			float y = RandomRange(0.0f, 1.0f);
			Data = new float[1000];
			for (int i = 0; i < 1000; ++i) {
				Data[i] = y + RandomRange(-0.01f, 0.01f);
			}
			Col = ImVec4(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), 0.5f);
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
		static int frames = 60;
		static int L = 0;
		static int F = 0;
		static double t1, t2;
		static int mode = BenchMode::Line;
		const char* names[] = { "Line","LineG","Shaded","Scatter","Bars" };

		static ImVector<BenchRecord> records;

		if (running) {
			F++;
			if (F == frames) {
				t2 = ImGui::GetTime();
				records.back().Data.push_back(ImPlotPoint(L, frames / (t2 - t1)));
				L += 5;
				F = 0;
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
			records.back().Data.reserve(max_items + 1);
			records.back().Mode = mode;
			records.back().AA = ImPlot::GetStyle().AntiAliasedLines;
			t1 = ImGui::GetTime();
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::Combo("##Mode", &mode, names, 4);
		ImGui::SameLine();

		ImGui::Checkbox("Anti-Aliased Lines", &ImPlot::GetStyle().AntiAliasedLines);
		if (was_running) { ImGui::PopItemFlag(); ImGui::PopStyleVar(); }

		ImGui::ProgressBar((float)L / (float)(max_items - 1));

		if (ImPlot::BeginPlot("##Bench", ImVec2(-1, 0), ImPlotFlags_NoChild | ImPlotFlags_CanvasOnly)) {
			ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
			ImPlot::SetupAxesLimits(0, 1000, 0, 1, ImGuiCond_Always);
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
						ImPlot::PlotLineG("##item", BenchmarkGetter, items[i].Data, 1000);
						ImGui::PopID();
					}
				}
				else if (mode == BenchMode::Shaded) {
					for (int i = 0; i < L; ++i) {
						ImGui::PushID(i);
						ImPlot::SetNextFillStyle(items[i].Col, 0.5f);
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
						ImPlot::SetNextFillStyle(items[i].Col, 0.5f);
						ImPlot::PlotBars("##item", items[i].Data, 1000);
						ImGui::PopID();
					}
				}
			}
			ImPlot::EndPlot();
		}
		static char buffer[64];
		if (ImPlot::BeginPlot("##Stats", ImVec2(-1, 0), ImPlotFlags_NoChild)) {
			ImPlot::SetupAxes("Items (1,000 pts each)", "Framerate (Hz)");
			ImPlot::SetupAxesLimits(0, 500, 0, 500, ImGuiCond_Always);
			for (int run = 0; run < records.size(); ++run) {
				if (records[run].Data.Size > 1) {
					sprintf(buffer, "B%d-%s%s", run + 1, names[records[run].Mode], records[run].AA ? "-AA" : "");
					ImVector<ImPlotPoint>& d = records[run].Data;
					ImPlot::PlotLine(buffer, &d[0].x, &d[0].y, d.Size, 0, 2 * sizeof(double));
				}
			}
			ImPlot::EndPlot();
		}
	}
}
