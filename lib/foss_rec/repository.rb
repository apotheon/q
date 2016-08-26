class Repository
  attr_accessor :path

  def initialize args={}
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

  def init
  end
end
