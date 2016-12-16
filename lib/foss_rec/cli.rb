require 'syck'

class CLI
  attr_reader :usage, :help_text, :config
  attr_accessor :name

  def initialize config=Hash.new
    @name = (config[:name] or 'fossrec')

    @usage = "USAGE: #{@name} COMMAND [ARGUMENTS]"

    @help_text = <<-EOF

      COMMANDS:

          init          Create a local repository.
          #{@name} init <projectname>

          clone         Clone a repository from FossRec.
          #{@name} clone <projectname>

          create        Create a repository on FossRec.
          (not currently implemented)

          help          Display this help text.
          #{@name} help

    EOF

    @config = configure config
  end

  private

  def configure config
    home = ENV['HOME']
    user = ENV['USER']

    path = File.join home, '.fossrec'
    repo = File.join path, 'repo'
    checkout = File.join home, 'src'

    Config.new.tap do |c|
      default_config = {
        local: {
          admin: user, checkout: checkout, path: path, repo: repo, user: user
        },

        remote: { account: user, admin: user, user: user }
      }

      if config[:file]
        c.file = config[:file]

        Syck.load_file c.file do |f|
          default_config.keys do |category|
            default_config[category].each_pair do |k, v|
              c[category][k] = f[category][k] or v
            end
          end
        end
      else
        c.file = File.join path, 'config'
        c.local = default_config[:local]
        c.remote = default_config[:remote]
      end
    end
  end

  Config = Struct.new :file, :local, :remote
end
