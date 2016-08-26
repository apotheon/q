require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

require_relative '../lib/foss_rec/repository.rb'

describe Repository do
  Repository.new(dir: 'tmp').tap do |repo|
    TMP = File.join Dir.pwd, 'tmp'
    REPO_FILE = 'project.fossil'
    FILE_PATH = File.join TMP, REPO_FILE

    before do
      Dir.mkdir 'tmp'
    end

    after do
      Dir.entries('tmp').each do |file|
        File.delete File.join('tmp', file) unless File.directory?(file)
      end

      Dir.delete 'tmp'
    end

    it 'initializes a new repository' do
      repo.path.must_equal FILE_PATH

      repo.init
      File.exist?(FILE_PATH).must_equal true, "failed to init #{FILE_PATH}"
    end
  end
end
