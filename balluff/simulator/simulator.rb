
require 'time'
require 'socket'
require 'optparse'
require 'thread'
require 'readline'

Encoding.default_internal, Encoding.default_external = ['BINARY'] * 2

class RFID 
  attr_reader :length
  
  def initialize(name, length = 1024)
    @name = name
    @length = length
    if File.exists? @name
      read
    else
      @data = '0' * @length
      write
    end
  end
  
  def pad(data)
    data[0,@length] + ('0' * (@length - data.length))
  end
  
  def data=(value)
    @data = pad(value)
  end
  
  def get_region(address, length)
    @data.slice(address, length)
  end
  
  def set_region(address, value)
    @data[address, value.length] = value
  end

  def write
    puts "Writing #{@data.inspect} to file #{@name}"
    File.open(@name, 'w') { |f| f.write(@data) }
  end
  
  def read
    @data = pad(File.read(@name))
    @length = @data.size
  end
end

class Reader
  attr_accessor :rfid
  
  ACK = 0x6.chr
  NAK = 0x15.chr
  STX = 0x2.chr
  
  def initialize(dev)
    @client = UNIXSocket.open(dev)
    @rfid = nil
    @running = false
  end
  
  def computeBCC(message)
    message.bytes.inject(0) { |s, v| s ^= v }
  end
  
  def checkBCC(message)
    bcc = computeBCC(message[0..-2])
    res = bcc == message.getbyte(-1)
    puts "BCC doesn't match: #{message.getbyte(-1)} != #{bcc}" unless res
    res
  end
  
  def addBCC(message)
    message + computeBCC(message).chr
  end
  
  def respond(message)
    response = addBCC(message)
    puts "Responding with: #{response.inspect}"
    @client.write(response)
  end
  
  def ack(value = '0')
    puts "Sending <ACK>#{value}"
    @client.write(ACK + value)
  end
  
  def nak(value = '7')
    puts "Sending <NAK>#{value}"
    @client.write(NAK + value)
  end
  
  def readHead(head)
    case head
    when '?'
      ack
      if @rfid
        puts "Responding with initial data"
        respond("H1" + @rfid.get_region(0,4))
      else
        respond('H?0000')
      end
    when '1'
      ack('1')
    when '2'
      ack('2')
    end
  end
  
  def writeRFID(address, length)
    if @rfid
      ack
      select([@client], nil, nil, 1)
      stx = @client.read_nonblock(1) 
      if (stx != STX)
        nak
      else
        data = ''
        length += 1 # for BCC
        puts "Waiting for #{length} bytes"
        while data.length < length
          data << @client.read(length - data.length)
          puts "Received: #{data.length} bytes"
        end
        if (length > @rfid.length)
          nak('7')
        else
          File.open("t.bin", 'w') { |f| f.write(data) }
          if (checkBCC(stx + data))
            @rfid.set_region(address, data.slice(0, length))
            @rfid.write
            ack
          else
            nak('8')
          end
        end
      end
    else
      nak('1')
    end
  end

  def readRFID(address, length)
    if @rfid
      ack
      puts "Waiting for STX..."
      select([@client], nil, nil, 1)
      stx = @client.read_nonblock(1) 
      puts "Received: #{stx.inspect} (#{stx == STX})"
      if (stx != STX)
        nak
      else
        puts "Getting region from rfid"
        message = addBCC(@rfid.get_region(address, length))
        puts "Writing message: #{message.inspect} #{message.length}"
        @client.write(message)
      end
    else
      nak('1')
    end
    
  end

  
  def handle(message)
    unless checkBCC(message)
      puts "Invalid BCC message #{computeBCC(message)} != #{message.getbyte(-1)}"
      nak('8')
    else
      case message
      when /^Q/
        respond("Q")
      
      when /^H(.)/
        readHead($1)
      
      when /^S/
        respond("S ")
      
      when /^R(\d{4,4})(\d{4,4})/
        readRFID($1.to_i, $2.to_i)
      when /^W(\d{4,4})(\d{4,4})/
        writeRFID($1.to_i, $2.to_i)
      else
        puts "Unhandled command: #{message}"
        nak
      end
    end
  rescue
    puts "Exception: #{$!}"
    puts $!.backtrace.join("\n")
  end
  
  def start
    Thread.new {
      puts "Flushing..."
      @client.read_nonblock(8192) if select([@client], nil, nil, 0)
      @running = true
      while @running
        puts "Waiting for data"
        select([@client])
        puts "reading..."
        message = @client.read_nonblock(8192)
        puts "Received #{message.inspect}"
        handle(message)
      end
    }
  end
  
  def stop
    @running = false
    @client.close
  end
end

if ARGV.length < 1
  puts "Usage: ruby simulator.rb <socket>"
  exit 1
end

reader = Reader.new(ARGV[0])
reader.start

loop do
  line = Readline::readline('> ')
  Readline::HISTORY.push(line)
  line.chomp!
  case line
  when /^q/i
    reader.stop
    exit 0
  when /^n/i
    reader.rfid = nil
    
  when /^([a-z1-9_]+)([ ]+\d+)?/
    # File name...
    file = "rfid#{$1}.bin"
    puts "Creating RFID for file: #{file} #{$2}"
    size = ($2 || 1024).to_i
    reader.rfid = RFID.new(file, size)
    
  else
    puts "Invalid command: #{line}"
  end
end
