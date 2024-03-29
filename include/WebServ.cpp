#include "WebServ.h"

String getContentType(String filename)
{
    if (Server.hasArg("download"))
    {
        return "application/octet-stream";
    }
    else if (filename.endsWith(".htm"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".html"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".css"))
    {
        return "text/css";
    }
    else if (filename.endsWith(".js"))
    {
        return "application/javascript";
    }
    else if (filename.endsWith(".png"))
    {
        return "image/png";
    }
    else if (filename.endsWith(".gif"))
    {
        return "image/gif";
    }
    else if (filename.endsWith(".jpg"))
    {
        return "image/jpeg";
    }
    else if (filename.endsWith(".ico"))
    {
        return "image/x-icon";
    }
    else if (filename.endsWith(".xml"))
    {
        return "text/xml";
    }
    else if (filename.endsWith(".pdf"))
    {
        return "application/x-pdf";
    }
    else if (filename.endsWith(".zip"))
    {
        return "application/x-zip";
    }
    else if (filename.endsWith(".gz"))
    {
        return "application/x-gzip";
    }
    return "text/plain";
}

bool handleFileRead(String path)
{
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/"))
    {
        path += "index.htm";
    }
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (filesystem->exists(pathWithGz) || filesystem->exists(path))
    {
        if (filesystem->exists(pathWithGz))
        {
            path += ".gz";
        }
        File file = filesystem->open(path, "r");
        Server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void handleFileUpload()
{
    if (Server.uri() != "/edit")
    {
        return;
    }
    HTTPUpload &upload = Server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if (!filename.startsWith("/"))
        {
            filename = "/" + filename;
        }
        Serial.print("handleFileUpload Name: ");
        Serial.println(filename);
        fsUploadFile = filesystem->open(filename, "w");
        filename = String();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
        if (fsUploadFile)
        {
            fsUploadFile.write(upload.buf, upload.currentSize);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (fsUploadFile)
        {
            fsUploadFile.close();
        }
        Serial.print("handleFileUpload Size: ");
        Serial.println(upload.totalSize);
    }
}

void handleFileDelete()
{
    if (Server.args() == 0)
    {
        return Server.send(500, "text/plain", "BAD ARGS");
    }
    String path = Server.arg(0);
    Serial.println("handleFileDelete: " + path);
    if (path == "/")
    {
        return Server.send(500, "text/plain", "BAD PATH");
    }
    if (!filesystem->exists(path))
    {
        return Server.send(404, "text/plain", "FileNotFound");
    }
    filesystem->remove(path);
    Server.send(200, "text/plain", "");
    path = String();
}

void handleFileCreate()
{
    if (Server.args() == 0)
    {
        return Server.send(500, "text/plain", "BAD ARGS");
    }
    String path = Server.arg(0);
    Serial.println("handleFileCreate: " + path);
    if (path == "/")
    {
        return Server.send(500, "text/plain", "BAD PATH");
    }
    if (filesystem->exists(path))
    {
        return Server.send(500, "text/plain", "FILE EXISTS");
    }
    File file = filesystem->open(path, "w");
    if (file)
    {
        file.close();
    }
    else
    {
        return Server.send(500, "text/plain", "CREATE FAILED");
    }
    Server.send(200, "text/plain", "");
    path = String();
}

void handleFileList()
{
    if (!Server.hasArg("dir"))
    {
        Server.send(500, "text/plain", "BAD ARGS");
        return;
    }

    String path = Server.arg("dir");
    Serial.println("handleFileList: " + path);
    Dir dir = filesystem->openDir(path);
    path = String();

    String output = "[";
    while (dir.next())
    {
        File entry = dir.openFile("r");
        if (output != "[")
        {
            output += ',';
        }
        bool isDir = false;
        output += "{\"type\":\"";
        output += (isDir) ? "dir" : "file";
        output += "\",\"name\":\"";
        if (entry.name()[0] == '/')
        {
            output += &(entry.name()[1]);
        }
        else
        {
            output += entry.name();
        }
        output += "\"}";
        entry.close();
    }

    output += "]";
    Serial.println(output);
    Server.send(200, "text/json", output);
}

void ServerInit(void)
{
    Server.on("/list", HTTP_GET, handleFileList);


    Server.on("/edit", HTTP_GET, []() {
        if (!handleFileRead("/edit.htm"))
        {
            Server.send(404, "text/plain", "FileNotFound");
        }
    });

    Server.on("/edit", HTTP_PUT, handleFileCreate);
    Server.on("/edit", HTTP_DELETE, handleFileDelete);
    Server.on(
        "/edit", HTTP_POST, []() {
            Server.send(200, "text/plain", "");
        },
        handleFileUpload);
    Server.onNotFound([]() {
        if (!handleFileRead(Server.uri()))
        {
            Server.send(404, "text/plain", "FileNotFound");
        }
    });

    Server.begin();
    Serial.println("HTTP server started");
}
