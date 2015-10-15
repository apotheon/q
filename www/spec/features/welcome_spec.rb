require 'rails_helper'

feature 'Welcome Page' do
  scenario 'a visitor opens the homepage' do
    visit root_path
    expect(page).to have_content 'Welcome to Fossil Record.'
  end
end
