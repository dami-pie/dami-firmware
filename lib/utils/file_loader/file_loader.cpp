#include "file_loader.h"

fs::File load_file(const char *file_path)
{
  if (!SPIFFS.begin())
  {
    log_e("An Error has occurred while mounting SPIFFS");
    return File();
  }
  return SPIFFS.open(file_path);
}