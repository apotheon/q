require 'syck'
require 'ostruct'

class Repository
  attr_accessor :admin, :path

  def initialize args={}
    if args[:admin]
      @admin = args[:admin]
    else
      @admin = ENV['USER']
    end

    if args[:dir].to_s.match /^\//
      @dir = args[:dir]
    else
      @dir = File.join Dir.pwd, args[:dir]
    end

    if args[:project]
      @project = args[:project]
    elsif args[:filename]
      @project = args[:filename].to_s.sub(/\.\w$/, '')
    else
      @project = 'project'
    end

    @ext = (args[:ext] or 'fossil')

    if args[:filename]
      @filename = args[:filename]
    else
      @filename = [@project, @ext].join '.'
    end

    if args[:path]
      @path = args[:path]
    else
      @path = File.join @dir, @filename
    end
  end

  def init initial_checkin=nil
    arguments = Array.new
    arguments << '--empty' unless initial_checkin
    arguments << "--admin-user #{admin}" if admin

    `fossil init #{arguments.join ' '} #{path}`
  end

  def info
    info = Hash.new

    Syck.load(`fossil info -R #{path}`).each_pair do |k, v|
      info[k.sub '-', '_'] = v
    end

    OpenStruct.new info
  end

  def checkins
    info.checkins
  end
end
