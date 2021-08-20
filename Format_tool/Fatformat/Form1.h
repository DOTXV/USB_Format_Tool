#pragma once
#include <Windows.h>
#include <WinIoCtl.h>
#include <devioctl.h>
#include <ntddscsi.h>
#include<Stdlib.h>
#include<Stdio.h>
#include<string.h>
#include<math.h>

#include"format_tool.h"

using namespace std;
using namespace System;
using namespace System::IO;

int disk_num[10] = { 0 };
int FAT_type;
int MBR_Flag;

extern volatile int progress;



namespace CppCLR_WinformsProjekt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung f�r Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzuf�gen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  comboBox1;
	protected:
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^ label9;
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode f�r die Designerunterst�tzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge鄚dert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(16, 61);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(200, 20);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(12, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 30);
			this->label1->TabIndex = 1;
			this->label1->Text = L"USB";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label2->Location = System::Drawing::Point(14, 98);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(109, 30);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Capacity";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(17, 130);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(199, 22);
			this->textBox1->TabIndex = 4;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label3->Location = System::Drawing::Point(222, 128);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(36, 24);
			this->label3->TabIndex = 5;
			this->label3->Text = L"GB";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label4->Location = System::Drawing::Point(14, 163);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(137, 30);
			this->label4->TabIndex = 6;
			this->label4->Text = L"File system";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(17, 196);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(200, 20);
			this->comboBox2->TabIndex = 7;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox2_SelectedIndexChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox1->Location = System::Drawing::Point(17, 231);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(85, 34);
			this->checkBox1->TabIndex = 8;
			this->checkBox1->Text = L"MBR";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(12, 277);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(142, 30);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Cluster size";
			this->label5->Click += gcnew System::EventHandler(this, &Form1::label5_Click);
			// 
			// comboBox3
			// 
			this->comboBox3->Location = System::Drawing::Point(16, 310);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(200, 20);
			this->comboBox3->TabIndex = 17;
			this->comboBox3->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox3_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(14, 344);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(186, 30);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Offset (sectors)";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(17, 377);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(199, 22);
			this->textBox2->TabIndex = 12;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label7->Location = System::Drawing::Point(218, 379);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(77, 20);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Reserved";
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::Salmon;
			this->button2->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(212, 420);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(84, 68);
			this->button2->TabIndex = 14;
			this->button2->Text = L"Format";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(12, 418);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(168, 30);
			this->label8->TabIndex = 15;
			this->label8->Text = L"Format status";
			// 
			// progressBar1
			// 
			this->progressBar1->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->progressBar1->Location = System::Drawing::Point(17, 461);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(166, 27);
			this->progressBar1->TabIndex = 16;
			this->progressBar1->Click += gcnew System::EventHandler(this, &Form1::progressBar1_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label9->Location = System::Drawing::Point(222, 306);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(34, 24);
			this->label9->TabIndex = 18;
			this->label9->Text = L"KB";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(308, 531);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Name = L"Form1";
			this->Text = L"USB Format Tool";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//////////////////////////////////////////////////
		array<DriveInfo^>^ allDriver;
		DriveInfo^ selected_disk;
		int sectorsize;
		//////////////////////////////////////////////////
		
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		//USB Info
		allDriver = System::IO::DriveInfo::GetDrives();
		DriveInfo^ disk;
		int c = 0;
		for (int i = 0; i < allDriver->Length; i++)
		{
			disk = allDriver[i];
			if (disk->IsReady == TRUE)
			{
				if (disk->Name == "C:\\" | disk->Name == "D:\\")
					continue;
				this->comboBox1->Items->Add(disk->Name);
				disk_num[c] = i;
				c++;
			}
		}

		//form load
		this->comboBox2->Items->Add("FAT32");
		this->comboBox2->Items->Add("exFAT");
		this->comboBox2->SelectedIndex = 0;
		this->textBox2->Text = "32";
		this->progressBar1->Maximum = 100;
		this->progressBar1->Minimum = 0;
		this->progressBar1->Step = 1;
		this->progressBar1->Visible = true;
		this->progressBar1->Value = 0;
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			selected_disk = allDriver[disk_num[this->comboBox1->SelectedIndex]];
			LONGLONG dsksize = LONGLONG::Parse(selected_disk->TotalSize.ToString());
			double temp = dsksize / 1000000000.00;
			double dspsize =Math::Round( temp, 2);
		this->textBox1->Text = dspsize.ToString();
	}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	this->comboBox1->Items->Clear();
	try
	{
		
	}
	catch (Win32Exception^ ex)
	{
		Console::WriteLine(ex->Message);
	}
	// Display each port name to the console.

}

private: System::Void comboBox2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->comboBox2->SelectedIndex == 0)
	{
		FAT_type = 0;                     //0 for FAT32
		this->comboBox3->Items->Clear();
		this->comboBox3->Items->Add("4");
		this->comboBox3->Items->Add("8");
		this->comboBox3->Items->Add("16");
		this->comboBox3->Items->Add("32");
		this->comboBox3->Items->Add("64");
		this->comboBox3->SelectedIndex = 1;
	}
	else
	{
		FAT_type = 1;                     //1 for exFAT
		this->comboBox3->Items->Clear();
		this->comboBox3->Items->Add("4");
		this->comboBox3->Items->Add("8");
		this->comboBox3->Items->Add("16");
		this->comboBox3->Items->Add("32");
		this->comboBox3->Items->Add("64");
		//this->comboBox3->Items->Add("128");
		this->comboBox3->SelectedIndex = 3;
	}
}
private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->checkBox1->Checked == true)
	{
		MBR_Flag = 1;
	}
	else
	{
		MBR_Flag = 0;
	}
}

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	//do the chack to able format process
	progress = 0;
	int res_sec = int::Parse(this->textBox2->Text);
	int cluster_size = int::Parse(this->comboBox3->Text);
	System::Windows::Forms::DialogResult result;
	MessageBoxButtons buttons = MessageBoxButtons::YesNo;
	result = MessageBox::Show(this, " Are You Sure?", "Format Tool", buttons, MessageBoxIcon::Warning);
	if (result == System::Windows::Forms::DialogResult::Yes)
	{
		//FAT type FAT32:0 , exFAT:1
		//MBR type with MBR:1 , without:0
		if (res_sec < 24) 
		{
			res_sec = 24;
		}
		format_main(selected_disk->Name, MBR_Flag, FAT_type, cluster_size, res_sec);
		progress = 100;
		MessageBoxButtons buttons2 = MessageBoxButtons::OK;
		result = MessageBox::Show(this, " Format Completed", "Format Tool", buttons2, MessageBoxIcon::Information);
	}
}//format button

private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	int count = progress;
	this->progressBar1->Value = count;
}
private: System::Void comboBox3_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void progressBar1_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
