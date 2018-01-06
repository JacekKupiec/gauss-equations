#!/usr/bin/env ruby

gen = Random.new
rows = ARGV[0].to_i
columns = ARGV[1].to_i
path = ARGV[2]

array = Array.new(rows) do |i| 
    Array.new(columns) { gen.rand*1000.0 }
end


File.open(path, 'w') do |f|
    f.puts rows
    array.each do |row|
        f.puts row.join(' ')
    end
end