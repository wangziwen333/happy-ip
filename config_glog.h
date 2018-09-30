#pragma once
#include <glog/logging.h>
#include <iostream>
#include <string>
using namespace google;
using namespace std;

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#elif __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#elif __linux__
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

#ifdef __linux__
#define   _MAX_PATH      260   /*   max.   length   of   full   pathname   	 */   
#define   _MAX_DRIVE     3     /*   max.   length   of   drive   component   	 */   
#define   _MAX_DIR       256   /*   max.   length   of   path   component   	 */   
#define   _MAX_FNAME     256   /*   max.   length   of   file   name   component */   
#define   _MAX_EXT       256   /*   max.   length   of   extension   component   */   

static void _split_whole_name(const char *whole_name, char *fname, char *ext)
{
  char *p_ext;

  p_ext = rindex(const_cast<char *>(whole_name), '.');
  if (NULL != p_ext)
  {
    strcpy(ext, p_ext);
    snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
  }
  else
  {
    ext[0] = '\0';
    strcpy(fname, whole_name);
  }
}

static void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
  char *p_whole_name;

  drive[0] = '\0';
  if (NULL == path)
  {
    dir[0] = '\0';
    fname[0] = '\0';
    ext[0] = '\0';
    return;
  }

  if ('/' == path[strlen(path)])
  {
    strcpy(dir, path);
    fname[0] = '\0';
    ext[0] = '\0';
    return;
  }

  p_whole_name = rindex(const_cast<char *>(path), '/');
  if (NULL != p_whole_name)
  {
    p_whole_name++;
    _split_whole_name(p_whole_name, fname, ext);

    snprintf(dir, p_whole_name - path + 1, "%s", path);
  }
  else
  {
    _split_whole_name(path, fname, ext);
    dir[0] = '\0';
  }
}

#endif

namespace happy
{
	namespace utils
	{
		static bool CreateDir(const char *pchDirName)
		{
			int nRet;
			int nLen = strlen(pchDirName);
			char chDirName[1024];

			if (!pchDirName)
			{
				return false;
			}
			memcpy(chDirName, pchDirName, nLen);

			//Append end character to '/'
			if (chDirName[nLen - 1] != '/')
			{
				chDirName[nLen] = '/';
				chDirName[nLen + 1] = '\0';
				nLen++;
			}

			for (int i = 0; i < nLen; i++)
			{
				if ('.' == chDirName[i])
				{
					i += 2;
				}
				if (chDirName[i] == '/')
				{

					chDirName[i] = '\0';

					// If directory is not exists, then create it
					nRet = ACCESS(chDirName, 0);
					if (nRet != 0)
					{
						nRet = MKDIR(chDirName);
						if (nRet != 0)
						{
							return false;
						}
					}
					chDirName[i] = '/';
				}
			}

			return true;
		}

		static string GetCurrentPath(const char* full_path)
		{
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR] = { 0 };
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(full_path, drive, dir, fname, ext);
			// If not found directory, default to current directory
			if (0 == dir[0])
			{
				strcpy(dir, "./");
			}
			return dir;
		}

		inline void ConfigGlog(const char* full_path, const int& minloglevel = GLOG_INFO, const int& min_severity = GLOG_INFO, const string& log_folder_name = "Log")
		{
			// Set google default parameter
			InitGoogleLogging(full_path);
			FLAGS_colorlogtostderr = true;
			//FLAGS_max_log_size = 1024; // max file size 1G
			FLAGS_log_dir = GetCurrentPath(full_path) + "//" + log_folder_name;
			FLAGS_minloglevel = minloglevel;
			SetStderrLogging(min_severity);
			CreateDir(FLAGS_log_dir.c_str());
		}
	}
}
