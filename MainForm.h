#pragma once

#include <msclr/marshal_cppstd.h>
#include <unordered_map>
#include <iostream>
#include "Patterns.h"

#define PLAY_KEY 0x51
#define RECORD_KEY 0xC0
#define DIVISIONS 20

//
//std::vector<std::string> SplitString(const std::string& str, char symbol)
//{
//	std::vector<std::string> splitStr;
//	std::stringstream ss(str);
//	std::string to;
//
//	while (std::getline(ss, to, symbol))
//	{
//		splitStr.push_back(to);
//	}
//
//	return splitStr;
//}

namespace NeuralGamer {

	bool m_Record;
	bool m_RecordPressed;

	bool m_Play;
	bool m_PlayPressed;

	bool m_KeyStates[256] = { false };

	POINT m_MousePosition;
	unsigned int m_Itterator = 0;
	time_t m_Time = 0;
	time_t m_DelayTime = 0;
	time_t m_HoldTime[256] = { 0 };
	POINT m_SelectedAreaStart;

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			

			ChangeScanArea(POINT{ 0, 0 }, POINT{ 500, 500 }, DIVISIONS);
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  screenCapture;
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::PictureBox^  neuraltracking;
	private: CPatterns* m_pPattern;
	private: RectangleF m_SelectionRect;
	private: int m_SelectingArea = 0;

	private: System::Windows::Forms::Button^  selectArea;
	protected:

	private: System::Void ChangeScanArea(POINT a, POINT b, Uint16 div)
	{
		m_SelectionRect.X = a.x > b.x ? b.x : a.x;
		m_SelectionRect.Y = a.y > b.y ? b.y : a.y;
		m_SelectionRect.Width = abs(b.x - a.x);
		m_SelectionRect.Height = abs(b.y - a.y);

		if (m_pPattern != NULL)
			delete m_pPattern;

		int scaleOfLayer = (m_SelectionRect.Width * m_SelectionRect.Height) / (div * div);
		std::vector<int> layers = { scaleOfLayer, scaleOfLayer, 2 };

		m_pPattern = new CPatterns(layers);
	}

	private: System::Void DrawingPreview()
	{
		this->screenCapture->Size = System::Drawing::Size(480, 270);

		if (screenCapture->BackgroundImage == nullptr)
			screenCapture->BackgroundImage = gcnew System::Drawing::Bitmap(screenCapture->Width, screenCapture->Height);

		Graphics^ buffGraphics = Graphics::FromImage(screenCapture->BackgroundImage);
		buffGraphics->Clear(System::Drawing::Color::White);

		// Making screenshot of workspace
		BYTE* pImg = AutoCO::Screenshot(
			POINT{ m_SelectionRect.X, m_SelectionRect.Y },
			POINT{ m_SelectionRect.X + m_SelectionRect.Width, m_SelectionRect.Y + m_SelectionRect.Height });

		float scaleOfLayer = (m_SelectionRect.Width * m_SelectionRect.Height) / (DIVISIONS * DIVISIONS);
		std::vector<float> input;

		// drawing image
		float coef = m_SelectionRect.Width > m_SelectionRect.Height ? (480 / m_SelectionRect.Width) : (270 / m_SelectionRect.Height);

		for (int i = 0; i < m_SelectionRect.Width; i += DIVISIONS)
			for (int j = 0; j < m_SelectionRect.Height; j += DIVISIONS)
			{
				RGBColor clr = AutoCO::GetPixelI(pImg, POINT{ i, j }, m_SelectionRect.Width);
				clr.ToBWC(1, 0, 0);
				System::Drawing::Brush^ brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::FromArgb(clr.r, clr.g, clr.b));
				buffGraphics->FillRectangle(brush, Drawing::Rectangle(i * coef, j * coef, coef * DIVISIONS + 1, coef * DIVISIONS + 1));
				
				input.push_back(clr.r / 255);

				delete brush;
			}

		m_pPattern->GetBrain()->Run(input);
		delete[] pImg;
		screenCapture->Refresh();
	}

	private: System::Void DrawingNeuralNetwork()
	{
		if (neuraltracking->BackgroundImage == nullptr)
			neuraltracking->BackgroundImage = gcnew System::Drawing::Bitmap(neuraltracking->Width, neuraltracking->Height);

		Graphics^ buffGraphics = Graphics::FromImage(neuraltracking->BackgroundImage);
		buffGraphics->Clear(System::Drawing::Color::White);

		// drawing neural
		for (int i = 0; i < m_pPattern->GetBrain()->LayerCount(); i++)
			for (int j = 0; j < m_pPattern->GetBrain()->GetLayer(i)->NeuronCount(); j++)
			{
				float inx = m_pPattern->GetBrain()->GetLayer(i)->GetNeuron(j)->GetValue();
				inx = min(1, max(0, inx));
				System::Drawing::Brush^ brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::FromArgb(inx * 255, inx * 255, inx * 255));
				buffGraphics->FillRectangle(brush, Drawing::Rectangle((j - floor(j / 35) * 35) * 5, i * 100 + floor(j / 35) * 5, 5, 5));

				delete brush;
			}

		neuraltracking->Refresh();
	}

	protected:

	protected:

	protected:

	private: System::ComponentModel::IContainer^  components;

	protected:

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->screenCapture = (gcnew System::Windows::Forms::PictureBox());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->neuraltracking = (gcnew System::Windows::Forms::PictureBox());
			this->selectArea = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->screenCapture))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->neuraltracking))->BeginInit();
			this->SuspendLayout();
			// 
			// screenCapture
			// 
			this->screenCapture->Location = System::Drawing::Point(12, 12);
			this->screenCapture->Name = L"screenCapture";
			this->screenCapture->Size = System::Drawing::Size(100, 100);
			this->screenCapture->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->screenCapture->TabIndex = 0;
			this->screenCapture->TabStop = false;
			// 
			// timer
			// 
			this->timer->Enabled = true;
			this->timer->Tick += gcnew System::EventHandler(this, &MainForm::timer_Tick);
			// 
			// neuraltracking
			// 
			this->neuraltracking->Location = System::Drawing::Point(699, 12);
			this->neuraltracking->Name = L"neuraltracking";
			this->neuraltracking->Size = System::Drawing::Size(233, 365);
			this->neuraltracking->TabIndex = 1;
			this->neuraltracking->TabStop = false;
			// 
			// selectArea
			// 
			this->selectArea->Location = System::Drawing::Point(585, 328);
			this->selectArea->Name = L"selectArea";
			this->selectArea->Size = System::Drawing::Size(98, 49);
			this->selectArea->TabIndex = 2;
			this->selectArea->Text = L"Select Area";
			this->selectArea->UseVisualStyleBackColor = true;
			this->selectArea->Click += gcnew System::EventHandler(this, &MainForm::selectArea_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::EnablePreventFocusChange;
			this->ClientSize = System::Drawing::Size(944, 389);
			this->Controls->Add(this->selectArea);
			this->Controls->Add(this->neuraltracking);
			this->Controls->Add(this->screenCapture);
			this->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->ForeColor = System::Drawing::Color::Black;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MainForm";
			this->Text = L"Neural Gamer";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->screenCapture))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->neuraltracking))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}

	#pragma endregion

	private: System::Void timer_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		GetCursorPos(&m_MousePosition);

		// handling all keys
		for (size_t i = 0; i < 256; i++)
		{
			if (GetAsyncKeyState(i))
			{
				if (!m_KeyStates[i])
				{
					// pressing key
					m_KeyStates[i] = true;

					if (i == 1 &&m_SelectingArea == 1)
					{
						m_SelectedAreaStart = m_MousePosition;
						m_SelectingArea = 2;
					}
				}
			}
			// releasing key
			else if (m_KeyStates[i])
			{
				m_KeyStates[i] = false;

				if (i == 1 && m_SelectingArea == 2)
				{
					ChangeScanArea(m_SelectedAreaStart, m_MousePosition, DIVISIONS);
					m_SelectingArea = 0;
				}
			}
		}

		DrawingPreview();

		DrawingNeuralNetwork();
	}

	private: System::Void selectArea_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (m_SelectingArea == 0)
			m_SelectingArea = 1;
	}
};
}
