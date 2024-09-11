DbgPrintLine("Running Test.lua, searching for wordpad")
processID = GetProcessID("Document - WordPad")

if processID == 0 then
    DbgPrintLine("Failed to get WordPad ProcessID")    
    return
else
    DbgPrintLine("WordPad ProcessID Found!\nAttempting to close WordPad")    
end

isProcessTerminated = TerminateProcess(processID)
if isProcessTerminated == 1 then
    DbgPrintLine("Terminated process!")
else
    DbgPrintLine("Failed to terminate process")
end