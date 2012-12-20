#pragma once

namespace Bootil 
{
	namespace Compression
	{
		class BOOTIL_EXPORT Job
		{
			public:

				virtual bool Finished() = 0;
				virtual void Destroy() = 0;
				virtual bool Success() = 0;
		};

		namespace FastLZ
		{
			BOOTIL_EXPORT bool Is( const void* pData, unsigned int iLength );
			BOOTIL_EXPORT bool Compress( const void* pData, unsigned int iLength, Bootil::Buffer& output );
			BOOTIL_EXPORT bool Extract( const void* pData, unsigned int iLength, Bootil::Buffer& output );

			// This sucks but I'm in a rush. TODO: Clean Up.
			BOOTIL_EXPORT Job* ExtractInThread( const void* pData, unsigned int iLength, unsigned int iTargetSize, const Bootil::BString& strOutputFile );
		}

		namespace LZMA
		{
			BOOTIL_EXPORT bool Compress( const void* pData, unsigned int iLength, Bootil::Buffer& output, int iLevel = 5, int iDictSize = 65536 );
			BOOTIL_EXPORT bool Extract( const void* pData, unsigned int iLength, Bootil::Buffer& output );

			// This sucks but I'm in a rush. TODO: Clean Up.
			BOOTIL_EXPORT Job* ExtractInThread( const void* pData, unsigned int iLength, const Bootil::BString& strOutputFile );
		}

		namespace Zip
		{
			class BOOTIL_EXPORT File 
			{
				public:

					struct Entry 
					{
						BString		Name;
						int			Size;
					};

					File();
					File( const BString& strFileName, const BString& strPassword = "" );
					File( void* pBuffer, unsigned int iSize, const BString& strPassword = "" );

					int GetNumItems();
					BString GetFileName( int i );

					bool IsOpen();

					bool AddFile( const BString& ZipName, const BString& LocalFileName );

					bool WriteAndClose( const BString& ZipName );
					bool WriteAndClose( Buffer& buffer );

					void ExtractToFolder( const BString& Folder );
					void ExtractFile( int i, const BString& FileName );

					void Close();

				protected:

					void Init();
					void EnsureWriter();

					void RefreshFiles();

					void* m_pRead;
					void* m_Write;

					typedef std::map<int, Entry> FileList;
					FileList	m_Files;
			};
		}
	}

}