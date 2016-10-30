#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void fillRunVectors(const Mat& bwImage, int& NumberOfRuns, vector <int>& stRun, vector <int>& enRun, vector <int>& rowRun)
{
	for (int i = 0; i < bwImage.rows; i++)
	{
		const uchar* rowData = bwImage.ptr <uchar>(i);

		if (rowData[0] == 255)
		{
			NumberOfRuns++;
			stRun.push_back(0);
			rowRun.push_back(i);
		}
		for (int j = 1; j < bwImage.cols; j++)
		{
			if (rowData[j - 1] == 0 && rowData[j] == 255)
			{
				NumberOfRuns++;
				stRun.push_back(j);
				rowRun.push_back(i);
			}
			else if (rowData[j - 1] == 255 && rowData[j] == 0)
			{
				enRun.push_back(j - 1);
			}
		}
		if (rowData[bwImage.cols - 1])
		{
			enRun.push_back(bwImage.cols - 1);
		}
	}
}

void firstPass(vector <int>& stRun, vector <int>& enRun, vector <int>& rowRun, int NumberOfRuns,
	vector <int>& runLabels, vector <pair <int, int >> &equivalences, int offset)
{
	runLabels.assign(NumberOfRuns, 0);
	int idxLabel = 1;
	int curRowIdx = 0;
	int firstRunOnCur = 0;
	int firstRunOnPre = 0;
	int lastRunOnPre = -1;
	for (int i = 0; i < NumberOfRuns; i++)
	{
		if (rowRun[i] != curRowIdx)
		{
			curRowIdx = rowRun[i];
			firstRunOnPre = firstRunOnCur;
			lastRunOnPre = i - 1;
			firstRunOnCur = i;

		}
		for (int j = firstRunOnPre; j <= lastRunOnPre; j++)
		{
			if (stRun[i] <= enRun[j] + offset && enRun[i] >= stRun[j] - offset)
			{
				if (runLabels[i] == 0) // 没有被标号过
					runLabels[i] = runLabels[j];
				else if (runLabels[i] != runLabels[j])// 已经被标号             
					equivalences.push_back(make_pair(runLabels[i], runLabels[j])); // 保存等价对
			}
		}
		if (runLabels[i] == 0) // 没有与前一列的任何run重合
		{
			runLabels[i] = idxLabel++;
		}

	}
}

void replaceSameLabel(vector <int>& runLabels, vector <pair <int, int >> &
	equivalence)
{
	int maxLabel = *max_element(runLabels.begin(), runLabels.end());
	vector <vector <bool >> eqTab(maxLabel, vector <bool>(maxLabel, false));
	vector <pair <int, int >> ::iterator vecPairIt = equivalence.begin();
	while (vecPairIt != equivalence.end())
	{
		eqTab[vecPairIt->first - 1][vecPairIt->second - 1] = true;
		eqTab[vecPairIt->second - 1][vecPairIt->first - 1] = true;
		vecPairIt++;
	}
	vector <int> labelFlag(maxLabel, 0);
	vector <vector <int >> equaList;
	vector <int> tempList;
	cout << maxLabel << endl;
	for (int i = 1; i <= maxLabel; i++)
	{
		if (labelFlag[i - 1])
		{
			continue;
		}
		labelFlag[i - 1] = equaList.size() + 1;
		tempList.push_back(i);
		for (vector <int>::size_type j = 0; j < tempList.size(); j++)
		{
			for (vector <bool>::size_type k = 0; k != eqTab[tempList[j] - 1].size(); k++)
			{
				if (eqTab[tempList[j] - 1][k] && !labelFlag[k])
				{
					tempList.push_back(k + 1);
					labelFlag[k] = equaList.size() + 1;
				}
			}
		}
		equaList.push_back(tempList);
		tempList.clear();
	}
	cout << equaList.size() << endl;
	for (vector <int>::size_type i = 0; i != runLabels.size(); i++)
	{
		runLabels[i] = labelFlag[runLabels[i] - 1];
	}
}

void bwLabel(const Mat& imgBw, Mat & imgLabeled)
{
	// 对图像周围扩充一格
	Mat imgClone = Mat(imgBw.rows + 1, imgBw.cols + 1, imgBw.type(), Scalar(0));
	imgBw.copyTo(imgClone(Rect(1, 1, imgBw.cols, imgBw.rows)));

	imgLabeled.create(imgClone.size(), imgClone.type());
	imgLabeled.setTo(Scalar::all(0));

	vector <vector <Point >> contours;
	vector <Vec4i> hierarchy;
	findContours(imgClone, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	vector <int> contoursLabel(contours.size(), 0);
	int numlab = 1;
	// 标记外围轮廓
	for (vector <vector <Point >> ::size_type i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][3] >= 0) // 有父轮廓
		{
			continue;
		}
		for (vector <Point>::size_type k = 0; k != contours[i].size(); k++)
		{
			imgLabeled.at <uchar>(contours[i][k].y, contours[i][k].x) = numlab;
		}
		contoursLabel[i] = numlab++;
	}
	// 标记内轮廓
	for (vector <vector <Point >> ::size_type i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][3] < 0)
		{
			continue;
		}
		for (vector <Point>::size_type k = 0; k != contours[i].size(); k++)
		{
			imgLabeled.at <uchar>(contours[i][k].y, contours[i][k].x) = contoursLabel[hierarchy[i][3]];
		}
	}
	// 非轮廓像素的标记
	for (int i = 0; i < imgLabeled.rows; i++)
	{
		for (int j = 0; j < imgLabeled.cols; j++)
		{
			if (imgClone.at <uchar>(i, j) != 0 && imgLabeled.at <uchar>(i, j) == 0)
			{
				imgLabeled.at <uchar>(i, j) = imgLabeled.at <uchar>(i, j - 1);
			}
		}
	}
	imgLabeled = imgLabeled(Rect(1, 1, imgBw.cols, imgBw.rows)).clone(); // 将边界裁剪掉1像素
}