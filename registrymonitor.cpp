#include "registrymonitor.h"

#if (defined (Q_OS_WIN) || defined (Q_OS_WIN32) || defined (Q_OS_WIN64))
#include <windows.h>
#include <tchar.h>
#endif

#include <stdio.h>
#include <string>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>

RegistryMonitor::RegistryMonitor()
{

}

void RegistryMonitor::run()
{
    exec();
}

int RegistryMonitor::exec()
{
#if (defined (Q_OS_WIN) || defined (Q_OS_WIN32) || defined (Q_OS_WIN64))
    while(1)
    {
        DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
                REG_NOTIFY_CHANGE_ATTRIBUTES |
                REG_NOTIFY_CHANGE_LAST_SET |
                REG_NOTIFY_CHANGE_SECURITY;

        HANDLE hEvent;
        HKEY   hMainKey = HKEY_CURRENT_USER;
        std::wstring path = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
        LPCTSTR   hSubKey = path.c_str();
        HKEY   hKey;
        LONG   lErrorCode;


        // Open a key.
        lErrorCode = RegOpenKeyEx(hMainKey, hSubKey, 0, KEY_NOTIFY, &hKey);
        if (lErrorCode != ERROR_SUCCESS)
        {
            _tprintf(TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);
            return 0;
        }

        // Create an event.
        hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (hEvent == NULL)
        {
            _tprintf(TEXT("Error in CreateEvent (%d).\n"), GetLastError());
            return 0;
        }

        // Watch the registry key for a change of value.
        lErrorCode = RegNotifyChangeKeyValue(hKey,
                                             TRUE,
                                             dwFilter,
                                             hEvent,
                                             TRUE);
        if (lErrorCode != ERROR_SUCCESS)
        {
            _tprintf(TEXT("Error in RegNotifyChangeKeyValue (%d).\n"), lErrorCode);
            return 0;
        }

        // Wait for an event to occur.
        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
        {
            _tprintf(TEXT("Error in WaitForSingleObject (%d).\n"), GetLastError());
            return 0;
        }
        else
        {
            _tprintf(TEXT("\nChange has occurred.\n"));
            emit regChanged("Change has occurred!");
        }

        // Close the key.
        lErrorCode = RegCloseKey(hKey);
        if (lErrorCode != ERROR_SUCCESS)
        {
            _tprintf(TEXT("Error in RegCloseKey (%d).\n"), GetLastError());
            return 0;
        }

        // Close the handle.
        if (!CloseHandle(hEvent))
        {
            _tprintf(TEXT("Error in CloseHandle.\n"));
            return 0;
        }
        sleep(1);
    }
#endif
}
