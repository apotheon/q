require 'minitest/spec'
require 'minitest/autorun'
require 'minitest/rg'

require_relative '../lib/foss_rec/cli'

describe CLI do
  CLI.new.tap do |cli|
    cli.config.tap do |config|
      HOME = ENV['HOME']
      USER = ENV['USER']
      REPO = File.join HOME, '.fossrec', 'repo'
      CHECKOUT = File.join HOME, 'src'

      config.file.tap do |file|
        it 'uses ~/.fossrec/config as default config file' do
          file.must_equal File.join HOME, '.fossrec/config'
        end
      end

      config.local.tap do |local|
        it 'uses USER as default local user' do
          local[:user].must_equal USER
        end

        it 'uses USER as default local admin' do
          local[:admin].must_equal USER
        end

        it 'uses HOME/.fossrec/repo as default repo path' do
          local[:repo].must_equal REPO
        end

        it 'uses HOME/src as default checkout path' do
          local[:checkout].must_equal CHECKOUT
        end
      end

      config.remote.tap do |remote|
        it 'uses USER as default remote user' do
          remote[:user].must_equal USER
        end

        it 'uses USER as default remote admin' do
          remote[:admin].must_equal USER
        end

        it 'uses USER as default remote account' do
          remote[:account].must_equal USER
        end
      end
    end

    it 'uses fossrec as default command' do
      cli.name.must_equal 'fossrec'
    end

    it 'provides basic usage help' do
      cli.usage.must_equal 'USAGE: fossrec COMMAND [ARGUMENTS]'
    end

    it 'provides command help' do
      [
        /init +Create a local repository\./,
        /fossrec init <projectname>/,
        /clone +Clone a repository from FossRec\./,
        /fossrec clone <projectname>/,
        /create +Create a repository on FossRec\./,
        /not currently implemented/,
        /help +Display this help text\./,
        /fossrec help/
      ].each do |regex|
        cli.help_text.must_match regex
      end
    end
  end
end
