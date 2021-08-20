#pragma once
#include <Windows.h>
#include <WinIoCtl.h>
#include <devioctl.h>
#include <ntddscsi.h>
#include<Stdlib.h>
#include<Stdio.h>
#include<string.h>
#include"format_tool.h"
#include<math.h>

using namespace std;
using namespace System;
using namespace System::IO;
//
int disk_num[10] = { 0 };
int FAT_type;
int MBR_Flag;
//int cluster_size;
extern volatile int progress;
//


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
	private: System::Windows::Forms::Button^  button1;
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
	private: System::ComponentModel::IContainer^  components;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
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
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(54, 56);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(200, 20);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 26.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(47, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 40);
			this->label1->TabIndex = 1;
			this->label1->Text = L"USB:";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(277, 48);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 31);
			this->button1->TabIndex = 2;
			this->button1->Text = L"load";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"標楷體", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(49, 83);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(73, 29);
			this->label2->TabIndex = 3;
			this->label2->Text = L"容量";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(54, 115);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 4;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Times New Roman", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(160, 110);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(45, 27);
			this->label3->TabIndex = 5;
			this->label3->Text = L"GB";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"標楷體", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(49, 148);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(133, 29);
			this->label4->TabIndex = 6;
			this->label4->Text = L"檔案系統";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(54, 180);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(200, 20);
			this->comboBox2->TabIndex = 7;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox2_SelectedIndexChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox1->Location = System::Drawing::Point(54, 239);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(98, 37);
			this->checkBox1->TabIndex = 8;
			this->checkBox1->Text = L"MBR";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Times New Roman", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(49, 294);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(220, 33);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Cluster Size (KB)";
			this->label5->Click += gcnew System::EventHandler(this, &Form1::label5_Click);
			// 
			// comboBox3
			// 
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Location = System::Drawing::Point(55, 330);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(200, 20);
			this->comboBox3->TabIndex = 10;
			this->comboBox3->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox3_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Times New Roman", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(51, 367);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(182, 33);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Offset(sectors)";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(54, 403);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 22);
			this->textBox2->TabIndex = 12;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Times New Roman", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(160, 402);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(86, 23);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Reserved";
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(250, 490);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(84, 68);
			this->button2->TabIndex = 14;
			this->button2->Text = L"Format";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Times New Roman", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(51, 472);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(172, 33);
			this->label8->TabIndex = 15;
			this->label8->Text = L"Format Status";
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(57, 520);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(166, 27);
			this->progressBar1->TabIndex = 16;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(367, 591);
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
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
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
		//form load
		this->comboBox2->Items->Add("FAT32");
		this->comboBox2->Items->Add("exFAT");
		this->comboBox2->SelectedIndex = 0;
		this->comboBox3->Items->Add("4");
		this->comboBox3->Items->Add("8");
		this->comboBox3->Items->Add("16");
		this->comboBox3->Items->Add("32");
		this->comboBox3->Items->Add("64");
		this->comboBox3->SelectedIndex = 2;
		this->textBox2->Text = "32";
		//cluster_size = 4;
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
			float temp = dsksize / 1000000000.00;
			float dspsize =Math::Round( temp, 2);
		this->textBox1->Text = dspsize.ToString();
	}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	this->comboBox1->Items->Clear();
	try
	{allDriver = System::IO::DriveInfo::GetDrives();
	DriveInfo^ disk;
	int c = 0;
	for(int i = 0;i< allDriver->Length;i++)
	{	
		disk = allDriver[i];
		if (disk->IsReady == TRUE)
		{
			this->comboBox1->Items->Add(disk->Name);
			disk_num[c] = i;
			c++;
		}
		else {}
	}
	}
	catch (Win32Exception^ ex)
	{Console::WriteLine(ex->Message);}

	// Display each port name to the console.

}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	//do the chack to able format process
	progress = 0;
	int res_sec = int::Parse(this->textBox2->Text);
	int cluster_size = int::Parse(this->comboBox3->Text);
	System::Windows::Forms::DialogResult result;
	MessageBoxButtons buttons = MessageBoxButtons::YesNo;
	result = MessageBox::Show(this, "確認格式化", "format_tool", buttons);
	if (result == System::Windows::Forms::DialogResult::Yes) {
		//bool format_main(System::String^ disk ,int MBR,int FATtype ,int cluster_size , int reserved_offset)
		//FAT type FAT32:0 , exFAT:1
		//MBR type with MBR:1 , without:0
		if (res_sec < 24) {
			res_sec = 24;
		}
		format_main(selected_disk->Name,MBR_Flag, FAT_type, cluster_size, res_sec);
		progress = 100;
		MessageBoxButtons buttons2 = MessageBoxButtons::OK;
		result = MessageBox::Show(this, "格式化完成", "format_tool", buttons2);
	}
}//format button
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (this->comboBox2->SelectedIndex == 0) { FAT_type = 0; }
	else { FAT_type = 1; }
	//FAT type 0->FAT32 , 1->exFAT
}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (this->checkBox1->Checked == true) { MBR_Flag = 1; }
	else { MBR_Flag = 0; }
}
private: System::Void comboBox3_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	//cluster_size = INT::Parse(this->comboBox3->Text);
}
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
};
}
