require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

require 'fileutils'

require_relative '../lib/foss_rec/repository.rb'

describe Repository do
  FIXTURE = File.join Dir.pwd, 'fixture'

  PROJECT = 'test'
  EXT = 'fossil'
  PROJECT_FILE = [PROJECT, EXT].join '.'
  REPOSITORY = File.join FIXTURE, PROJECT_FILE

  TMP = File.join Dir.pwd, 'tmp'
  DIR = File.join TMP, 'test_project'

  CONFIG = {
    admin: 'admin',
    dir: FIXTURE,
    project: PROJECT,
    ext: EXT,
  }

  before do
    [TMP, DIR].each {|directory| Dir.mkdir directory }
  end

  after do
    [DIR, TMP].each {|directory| FileUtils.remove_dir directory }
  end

  Repository.new(CONFIG).tap do |repo|
    it 'opens a working directory' do
      repo.filename.must_equal PROJECT_FILE
      repo.open DIR, :nested

      File.exist?(
        File.join(DIR, 'README')
      ).must_equal true, "no README found in #{DIR}"
    end
  end
end
