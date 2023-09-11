#include "ProcessHandler.h"
#include <iostream>
#include "RunnableThread.h"
#include <wx/wxcrtvararg.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include "Application.h"

BEGIN_EVENT_TABLE(ProcessHandler, wxEvtHandler)
		EVT_END_PROCESS(wxID_ANY, ProcessHandler::OnProcessTerminated)
END_EVENT_TABLE()
int ID_PROC = wxNewId();

ProcessHandler::ProcessHandler(wxApp* app, const wxString& command) : m_Process(nullptr), running(false),
                                                                            errorProcessReaderThread(nullptr),
                                                                            outputProcessReaderThread(nullptr),
                                                                            m_Cmd(command),
                                                                            app(app)
{
}

void ProcessHandler::OnProcessTerminated(wxProcessEvent& e)
{
	running = false;
	wxTheApp->ExitMainLoop();
	m_Process->CloseOutput();
}

void ProcessHandler::WriteIntoProcess(const wxString& command) const
{
	wxOutputStream* processOutputStream = m_Process->GetOutputStream();
	if (processOutputStream)
	{
		wxTextOutputStream textOutputStream(*processOutputStream);
		// I always prefere to use TextOutputStream.WriteString() but you can also use the low level wxOutputStream.WriteAll
		if (command.EndsWith(wxT('\n')))
			processOutputStream->WriteAll(command.c_str(), command.size());
		else
		{
			textOutputStream.WriteString(command + wxT('\n'));
			textOutputStream.Flush();
		}
	}
}

void ProcessHandler::Start()
{
	// The handler will receive EVT_END_PROCESS, by default the Event id is wxID_ANY
	// if I want to change that I have to add the second argument to our custom Id, and also
	// change it in EVT_END_PROCESS first argument
	m_Process = new wxProcess(this);

	m_Process->Redirect();

	// wxEXEC_SHOW_CONSOLE targets MSW only
	const long pid = wxExecute(m_Cmd, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, m_Process);


	// Process Output Stream
	outputProcessReaderThread = new RunnableThread([&]() -> void*
	{
		char buffer[4096];
		size_t bytesRead = 0;

		wxInputStream* in = m_Process->GetInputStream();

		while ((bytesRead = in->Read(&buffer, sizeof(buffer)).LastRead()) > 0)
		{
			app->OnProcOutput(wxString(buffer, 0, bytesRead));
		};
		return nullptr;
	});
	outputProcessReaderThread->Create();
	outputProcessReaderThread->Run();


	// Process Error Stream
	errorProcessReaderThread = new RunnableThread([&]() -> void*
	{
		char buffer[4096];
		size_t bytesRead = 0;
		wxInputStream* in = m_Process->GetErrorStream();
		while ((bytesRead = in->Read(&buffer, sizeof(buffer)).LastRead()) > 0)
		{
			app->OnProcOutput(wxString(buffer, 0, bytesRead));
		}

		return nullptr;
	});

	errorProcessReaderThread->Create();
	errorProcessReaderThread->Run();
}

void ProcessHandler::FinishSync() const
{
	m_Process->CloseOutput();
	/* Joinable threads must be deleted manually
	outputProcessReaderThread->Wait();
	errorProcessReaderThread->Wait();


	delete outputProcessReaderThread;
	delete errorProcessReaderThread;
	*/
}
