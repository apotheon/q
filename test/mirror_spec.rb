require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

require 'fileutils'

require_relative '../lib/foss_rec/repository.rb'
require_relative '../lib/foss_rec/mirror'

describe Mirror do
  FIXTURE = File.join Dir.pwd, 'fixture'

  PROJECT = 'test'
  EXT = 'fossil'
  PROJECT_FILE = [PROJECT, EXT].join '.'
  REPOSITORY = File.join FIXTURE, PROJECT_FILE

  TMP = File.join Dir.pwd, 'tmp'
  DIR = File.join TMP, 'test_project'

  RCONFIG = {
    admin: 'admin',
    dir: FIXTURE,
    project: PROJECT,
    ext: EXT,
  }

  GITDIR = File.join TMP, 'git'

  MCONFIG = {
    marks: DIR,
    git: GITDIR
  }

  before do
    [TMP, DIR, GITDIR].each {|directory| Dir.mkdir directory }
    `git init #{GITDIR}`
  end

  after do
    [DIR, GITDIR, TMP].each do |directory|
      FileUtils.remove_dir directory
    end
  end

  Repository.new(RCONFIG).tap do |repo|
    Mirror.new(MCONFIG).tap do |mirror|
      it 'exports Git format' do
        repo.open DIR, :nested

        mirror.create_mirror

        File.exist?(
          File.join(GITDIR, 'README')
        ).must_equal true, "no README found in #{GITDIR}"
      end
    end
  end
end
