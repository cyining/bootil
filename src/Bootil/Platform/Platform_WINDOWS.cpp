
#ifdef _WIN32

#define _WIN32_WINNT 0x05000000

#include "Bootil/Bootil.h"
#include <windows.h>
#include <direct.h>
#include <process.h> 

namespace Bootil 
{
	namespace Platform
	{
		BOOTIL_EXPORT BString LastError()
		{
			char strMessageBuffer[1024];
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), strMessageBuffer, 1024, NULL );
			BString strMessage = strMessageBuffer;
			String::Util::Trim( strMessage, "\n" );
			String::Util::Trim( strMessage, "\r" );
			return strMessage;
		}

		BString FullProgramName( void )
		{
			char strCurrentExe[MAX_PATH];
			GetModuleFileNameA( NULL, strCurrentExe, sizeof(strCurrentExe) );
			BString str = strCurrentExe;
			String::File::FixSlashes( str );
			return str;
		}

		BString ProgramName( void )
		{
			BString str = FullProgramName();
			String::File::ExtractFilename( str );
			String::File::StripExtension( str );
			return str;
		}

		BString ProgramFolder( void )
		{
			BString strFolder = FullProgramName();
			String::File::StripFilename( strFolder );
			return strFolder;
		}

		BOOTIL_EXPORT BString CurrentDir( void )
		{
			BString strCurretnntDir = "AGH";
			// TODO.
			return strCurretnntDir;
		}

		BOOTIL_EXPORT BString TemporaryDir( void )
		{
			char buffer[512];
			GetTempPathA( 512, buffer );
			BString str = buffer;
			String::File::CleanPath( str );
			return str;
		}

		BString CurrentUserName( void )
		{
			DWORD bufCharCount = 128;
			TCHAR infoBuf[128];
			
			if ( GetUserName( infoBuf, &bufCharCount ) )
				return infoBuf;

			return "<UNKNOWN>";
		}

		BOOTIL_EXPORT void SetupAssociation( BString ext )
		{
			//Write our file association to the registry -NOTE: must run in administrator mode once to work
			HKEY hKey;
			LPDWORD dwDisp = 0;

			BString strmv = "\"" + FullProgramName() + "\" %1"; 
			String::File::ToWindowsSlashes( strmv );

			BString keyName = ext + "\\shell\\open\\command";
			LONG lRet  = RegCreateKeyEx( HKEY_CLASSES_ROOT, keyName.c_str(), 0L,NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,dwDisp);
			if(!lRet)
				RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)strmv.c_str(), strmv.length()+1);
		}

		BOOTIL_EXPORT unsigned int DesktopWidth( void )
		{
			return GetSystemMetrics(SM_CXSCREEN);
		}

		BOOTIL_EXPORT unsigned int DesktopHeight( void )
		{
			return GetSystemMetrics(SM_CYSCREEN);
		}

		BOOTIL_EXPORT bool Popup( const BString& strName, const BString& strText )
		{
			MessageBoxA( NULL, strText.c_str(), strName.c_str(), MB_OK | MB_ICONASTERISK );
			return true;
		}

		BOOTIL_EXPORT void DebuggerOutput( const BString& strText )
		{
			OutputDebugString( strText.c_str() );
		}

		BOOTIL_EXPORT void OpenWebpage( const BString& strURL )
		{
			ShellExecute( NULL, "open", strURL.c_str(),	NULL, NULL, SW_SHOWNORMAL );
		}

		BOOTIL_EXPORT int FindFiles( String::List* files, String::List* folders, const BString& strFind, bool bUpUpFolders )
		{
			WIN32_FIND_DATAA FindFileData;
			HANDLE hFind;
			unsigned int iFiles = 0;

			hFind = FindFirstFileExA( strFind.c_str(), FindExInfoStandard, &FindFileData, FindExSearchNameMatch, NULL, 0 );

			while ( hFind != INVALID_HANDLE_VALUE )
			{
				BString strName = FindFileData.cFileName;

				if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && folders )
				{
					bool bInclude = true;

					if ( bUpUpFolders || ( (strName != ".") && (strName != ".." ) ) )
					{
						folders->push_back( strName );
						iFiles++;
					}

				}
				else if ( files )
				{
					files->push_back( strName );
					iFiles++;
				}

				if ( !FindNextFileA( hFind, &FindFileData ) )
				{
					break;
				}
			}

			FindClose( hFind );
			return iFiles;
		}

		BOOTIL_EXPORT void ChangeDir( const BString& strName )
		{
			chdir( strName.c_str() );
		}

		BOOTIL_EXPORT void StartProcess( const BString& strProcess, bool AndWait )
		{
			BString strFileName = String::File::GetExtractFilename( strProcess );

			const char *args[32];
			args[0] = strFileName.c_str();
			args[1] = NULL;

			int iFlag = P_NOWAIT;
			if ( AndWait ) iFlag = P_WAIT;

			spawnv( P_WAIT, strProcess.c_str(), args );
		}
		
	}
}

#endif