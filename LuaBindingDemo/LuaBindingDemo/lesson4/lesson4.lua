avg, sum = mylib.average(10, 20, 30, 40, 50)
print("The average is ", avg)
print("The sum is ", sum)


io.write('[Lua] Calling the C functionn\n')
a,b = mylib.displayLuaFunction(12, 3.141592, 'hola')
-- print the return values
io.write('[Lua] The C function returned <' .. a .. '> and <' .. b .. '>\n')
