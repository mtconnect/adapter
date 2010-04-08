
require 'socket'

$s = UNIXSocket.new('/tmp/adc_device')
line = ""
over = ""

def send(v)
  sleep(4)
  puts "Writing #{v.inspect}: #{$s.write(v)}"
end

count = 0
while select([$s])
  c = $s.read_nonblock(256)
  line << c
  if c =~ /\r/
    line, over = line.split("\r")
    puts "Received: #{line.inspect}"
    
    case line
    when "V"
      send("V30\r")

    when "I"
      send("IFF00\r")

    when /^O/
      send("O\r")

    when /^Q(\d)/
      send("Q#{$1}%03X\r" % count)
      
    when /^U(\d)/
      send("U#{$1}00F\r")

    else
      send("Error\r")
    end

    line = over.to_s
    count += 1
  end
end
