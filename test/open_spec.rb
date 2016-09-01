require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

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
    Dir.mkdir TMP
    Dir.mkdir DIR
  end

  after do
    Dir.entries(DIR).each do |file|
      File.delete File.join(DIR, file) unless File.directory? file
    end

    Dir.delete DIR

    Dir.entries(TMP).each do |file|
      File.delete File.join(TMP, file) unless File.directory? file
    end

    Dir.delete TMP
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
