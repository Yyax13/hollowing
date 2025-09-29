# Linux Process Hollowing with PTRACE

This repository contains a *Proof-of-Concept* (PoC) for **Process Hollowing** on Linux, utilizing the `ptrace` API to manipulate process memory and registers. The goal is to inject and execute malicious shellcode within a legitimate process, thereby disguising its execution.

The shellcode used is a **Metasploit Meterpreter** payload (`linux/x64/meterpreter/reverse_tcp`), pre-configured for a *reverse shell*.

## Usage

### Compile

To get everything ready, use the `Makefile`:

```bash
make build    
make example
```

### Run Hollowing

#### Generic Tool (`hollowing`)

To inject shellcode into any binary, use:

```bash
sudo ./hollowing <path_to_target_binary>
# Example: sudo ./hollowing /bin/sleep
```
This will launch the target binary, inject the shellcode, and execute Meterpreter.

#### Advanced Example

To see the donut spinning while the hollowing happens under the hood (using `/usr/bin/firefox` as the default target):

```bash
./example
```

### Configure Listener

To receive the Meterpreter connection, set up a listener. The `Makefile` already has a task for this:

```bash
sudo make listen-example
```

> Run with `sudo` because port 443 is a privileged port.

### Generate Shellcode (Optional)

If you want to generate your own shellcode, you can use `msfvenom`. The `Makefile` has a task for that too:

```bash
make shellcode
```