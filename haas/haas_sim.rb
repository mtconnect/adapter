
require 'socket'

$s = UNIXSocket.new('/tmp/adc_device')
line = ""
over = ""

def send(v)
  sleep(4)
  s = "\002#{v}\027\r\n>"
  puts "Writing #{v.inspect}: #{$s.write(s)}"
end

count = 0
while select([$s])
  c = $s.read_nonblock(256)
  line << c
  if c =~ /\r/
    line, over = line.split("\r")
    puts "Received: #{line.inspect}"
    
    case line
    when "Q100"
      send("S/N, 12345678")

    else
      send("Error\r")
    end

    line = over.to_s
    count += 1
  end
end
