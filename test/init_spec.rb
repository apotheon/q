require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

require_relative '../lib/foss_rec/repository.rb'

describe Repository do
  TMP = File.join Dir.pwd, 'tmp'
  EXT = '.fossil'

  before do
    Dir.mkdir TMP
  end

  after do
    Dir.entries(TMP).each do |file|
      File.delete File.join(TMP, file) unless File.directory? file
    end

    Dir.delete TMP
  end

  Repository.new(dir: 'tmp').tap do |repo|
    project = 'project'
    repo_file = project + EXT
    file_path = File.join TMP, repo_file

    it 'initializes a new, empty, unnamed repository' do
      repo.path.must_equal file_path
      repo.init

      File.exist?(file_path).must_equal true, "failed to init #{file_path}"
      repo.checkins.must_equal 0, "failure: #{repo.checkins} checkins found"
      repo.admin.must_equal ENV['USER'], "failure: admin is #{repo.admin}"
    end
  end

  it 'initializes a new, empty, named repository' do
    project = 'test'
    repo_file = project + EXT
    file_path = File.join TMP, repo_file

    repo = Repository.new(dir: 'tmp', project: project)
    repo.path.must_equal file_path

    repo.init

    File.exist?(file_path).must_equal true, "failed to init #{file_path}"
  end
end
