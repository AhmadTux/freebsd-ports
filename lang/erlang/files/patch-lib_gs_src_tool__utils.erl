
$FreeBSD$

--- lib/gs/src/tool_utils.erl.orig	Tue Oct 12 22:14:23 2004
+++ lib/gs/src/tool_utils.erl	Tue Oct 12 22:17:17 2004
@@ -27,6 +27,9 @@
 -export([file_dialog/1]).
 -export([notify/2, confirm/2, confirm_yesno/2, request/2]).
 
+%% Browser executable list (openURL command line protocol required)
+-define(BROWSERS, ["netscape", "mozilla", "MozillaFirebird", "opera"]).
+
 %%----------------------------------------------------------------------
 %% open_help(GS, File)
 %%   GS = gsobj()  (GS root object returned by gs:start/0,1)
@@ -65,7 +68,8 @@
 	      local ->
 		  case os:type() of
 		      {unix,_AnyType} ->
-			  "netscape -remote \"openURL(file:" ++ File ++ ")\"";
+			  unix_url_command("file:" ++ File);
+
 		      {win32,_AnyType} ->
 			  "start " ++ filename:nativename(File);
 
@@ -77,7 +81,7 @@
 	      remote ->
 		  case os:type() of
 		      {unix,_AnyType} ->
-			  "netscape -remote \"openURL(" ++ File ++ ")\"";
+			  unix_url_command("file:" ++ File);
 
 		      {win32,_AnyType} ->
 			  "netscape.exe -h " ++ regexp:gsub(File,"\\\\","/");
@@ -337,3 +341,54 @@
     [Last];
 insert_newlines(Other) ->
     Other.
+
+
+%% find_browser(BrowserList) => string() | false
+%%   BrowserList - [string()]
+%% Given a list of basenames, find the first available executable.
+
+find_browser([]) ->
+    false;
+
+find_browser([H | T]) ->
+    case os:find_executable(H) of
+        false ->
+          find_browser(T);
+        Browser ->
+          Browser
+    end.
+
+%% unix_url_command(URL) => string()
+%%   URL - string()
+%% Open an URL, using a browser which supports the openURL command
+%% line protocol. If no browser is found, the empty string will be
+%% returned.
+
+unix_url_command(URL) ->
+    Template = "BROWSER -remote \"openURL(" ++ URL ++ ")\" || BROWSER " ++ URL ++ "&",
+
+    case os:getenv("BROWSER") of
+	false ->
+	    %% look for a compatible browser
+	    case find_browser(?BROWSERS) of
+		false ->
+		    "";
+		Browser ->
+		    case regexp:gsub(Template, "BROWSER", Browser) of
+			{ok, Command, 0} ->
+			    %% Template does not contain "BROWSER" placeholder
+			    "";
+			{ok, Command, _} ->
+			    Command
+		    end
+	    end;
+
+	Value ->
+	    case regexp:gsub(Template, "BROWSER", Value) of
+		{ok, Command2, 0} ->
+		    %% no placeholder
+		    "";
+		{ok, Command2, _} ->
+		    Command2
+	    end
+    end.
