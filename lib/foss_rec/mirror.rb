class Mirror
  def initialize args=Hash.new, repo=nil
    if args[:git].to_s.match /^\//
      @dir = args[:git]
    else
      @dir = File.join Dir.pwd, args[:git]
    end

    if args[:marks].to_s.match /^\//
      @marks = args[:marks]
    else
      @dir = File.join Dir.pwd, args[:marks]
    end

    if args[:fossil_marks].to_s.match /^\//
      @fossil_marks = args[:fossil_marks]
    elsif args[:fossil_marks] and @marks
      @fossil_marks = File.join @marks, args[:fossil_marks]
    elsif @marks
      @fossil_marks = File.join @marks, 'fossil.marks'
    elsif args[:fossil_marks]
      @fossil_marks = args[:fossil_marks]
    else
      @fossil_marks = 'fossil.marks'
    end

    @repo = repo
  end

  def create_mirror skipmarks=false
    fossil_args = ['--git']
    #fossil_args << "--export-marks #{@fossil_marks}" unless skipmarks
    fossil_args << (@repo or Dir.pwd)

    git_args = ['fast-import']
    #git_args << "--export-marks=#{@git_marks}" unless skipmarks

    Dir.chdir @dir
    `fossil export #{fossil_args.join ' '} | git #{git_args.join ' '}`
  end

  def update_mirror
  end
end
