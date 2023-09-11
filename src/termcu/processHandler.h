#pragma once
#include <wx/app.h>

class RunnableThread;
class wxProcess;
class wxProcessEvent;
class Application;

class ProcessHandler : public wxEvtHandler {
public:
  ProcessHandler(wxApp *app, const wxString &command);

  void OnProcessTerminated(wxProcessEvent &e);

  void WriteIntoProcess(const wxString &command) const;
  void Start();
  void FinishSync() const;

private:
  wxProcess *m_Process;
  volatile bool running = false;
  RunnableThread *errorProcessReaderThread;
  RunnableThread *outputProcessReaderThread;

  wxString m_Cmd;
  Application *app;

  DECLARE_EVENT_TABLE()
};
