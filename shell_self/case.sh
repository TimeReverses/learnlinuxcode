#! /bin/sh
yyyyyyyyy

xxxxxxxxx
echo "please input your options."
read 00OPTION00
case "$00OPTION00" in
        start)
                echo "game start."
                echo "first level....";;
        pause)
                echo "game pause."
                echo "wait restart.....";;
        restart)
                echo "restart...";;
        end)
                echo "game over."
                echo "next time you will be better..";;
        *)
                echo "this option is error,sorry."
                return 1;;
esac
echo "bye bye"
return 0
