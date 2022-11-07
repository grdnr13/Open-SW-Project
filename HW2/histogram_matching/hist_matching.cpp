#include "hist_func.h"

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void change_one_to_one(G* trans_func_eq2, G* trans_func_match);
void hist_match(Mat& input, Mat& output, G* trans_func);


int main() {
	//Mat input = imread("C:\\EWHA\\OpenSWProject\\Lec3\\skeleton code\\input.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
	//Mat reference = imread("C:\\EWHA\\OpenSWProject\\Lec3\\skeleton code\\reference_img.jpg", CV_LOAD_IMAGE_COLOR);
	Mat reference = imread("reference_img2.jpg", CV_LOAD_IMAGE_COLOR);
	if (!reference.data) {
		std::cout << "reference image not found\n";
		return -1;
	}
	Mat input_gray;
	Mat reference_gray;

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale
	cvtColor(reference, reference_gray, CV_RGB2GRAY);	// convert RGB to Grayscale

	Mat equalized = input_gray.clone();
	Mat equalized_ref = reference_gray.clone();
	Mat matched = input_gray.clone(); //결과물


	// PDF or transfer function txt files
	FILE* f_PDF;
	FILE* f_matched_PDF_gray;
	FILE* f_trans_func_match;

	FILE* f_ref_PDF;

	fopen_s(&f_PDF, "PDF.txt", "w+");
	fopen_s(&f_ref_PDF, "ref_PDF.txt", "w+");
	fopen_s(&f_matched_PDF_gray, "matched_PDF_gray.txt", "w+");
	fopen_s(&f_trans_func_match, "trans_func_match.txt", "w+");

	float* PDF = cal_PDF(input_gray);	// PDF of Input image(Grayscale) : [L]
	float* CDF = cal_CDF(input_gray);	// CDF of Input image(Grayscale) : [L]

	float* ref_PDF = cal_PDF(reference_gray);
	float* ref_CDF = cal_CDF(reference_gray);

	G trans_func_eq[L] = { 0 };			// transfer function
	G trans_func_eq2[L] = { 0 };		//G(z)
	G trans_func_match[L] = { 0 };		//matching function

	hist_eq(input_gray, equalized, trans_func_eq, CDF);
	//G(z)만들기
	hist_eq(reference_gray, equalized_ref, trans_func_eq2, ref_CDF);

	//G(z) 일대일대응으로 변경, trans_func_match 만들기
	change_one_to_one(trans_func_eq2, trans_func_match);
	//match
	hist_match(input_gray, matched, trans_func_match);


	// histogram matching on grayscale image
	float* matched_PDF_gray = cal_PDF(matched);

	// equalized PDF (grayscale)

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
		fprintf(f_ref_PDF, "%d\t%f\n", i, ref_PDF[i]);
		fprintf(f_matched_PDF_gray, "%d\t%f\n", i, matched_PDF_gray[i]);

		// write transfer functions
		fprintf(f_trans_func_match, "%d\t%d\n", i, trans_func_match[i]);
	}

	// memory release
	free(PDF);
	free(CDF);
	free(ref_PDF); free(ref_CDF);
	fclose(f_PDF);
	fclose(f_matched_PDF_gray);
	fclose(f_trans_func_match);

	////////////////////// Show each image ///////////////////////

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	//namedWindow("Reference", WINDOW_AUTOSIZE);
	//imshow("Reference", reference_gray);

	namedWindow("Matched", WINDOW_AUTOSIZE);
	imshow("Matched", matched);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

void change_one_to_one(G* trans_func_eq2, G* trans_func_match) {
	trans_func_match[trans_func_eq2[0]] = 0;
	for (int i = 1; i < L; i++) {
		if (trans_func_match[trans_func_eq2[i]] == 0)
			trans_func_match[trans_func_eq2[i]] = i;
		else {
			if(trans_func_eq2[i] + 1<256)
				trans_func_match[trans_func_eq2[i] + 1] = i;
		}
	}
	for (int i = 1; i < L; i++) {
		if (trans_func_match[i] == 0)
			trans_func_match[i] = min((trans_func_match[i-1] + 1),255);
	}

}

// histogram equalization
void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}

// histogram matching
void hist_match(Mat& input, Mat& output, G* trans_func) {

	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			output.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}